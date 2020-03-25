#include "httpobject.h"
#include <ctype.h>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include <QDataStream>
#include <QIODevice>
#include <stdlib.h>

HTTPObject::HTTPObject()
{

}
HTTPObject::HTTPObject(const QString phrase)
{
    if(!parseHTTPString(phrase))
    {
        qWarning() << "Somthing went wrong creating the objet. Data might be incomplite";
    }
}

bool HTTPObject::parseHTTPString(const QString phrase)
{
    if(phrase.indexOf("GET ") != 0)
    {
        return false;
    }
    QStringList phrases = phrase.split("\r\n");
    QStringList getReqs = phrases[0].split(" ");
    if(getReqs.length() < 3)
    {
        return false;
    }
    _getAddress = getReqs[1];
    if(getReqs[2].length() > 4)
    {
        int slashDex = getReqs[2].indexOf("/");
        unsigned char ver[2];
        ver[0] = getReqs[2][slashDex+1].toLatin1()-48;
        ver[1] = getReqs[2][slashDex+3].toLatin1()-48;
        _HTTPVersion = *reinterpret_cast<unsigned short *>(ver);
    }
    phrases.removeAt(0);
    foreach(QString string,phrases)
    {
        QString arg(string.left(string.indexOf(":")+1));
        QString temp(string.right((string.length()-arg.length())-1));

        if(arg == "Host:")
        {
            _host = temp;
        }
        else if(arg == "User-Agent:")
        {
            _userAgent = temp;
        }
        else if(arg == "Accept:")
        {
            _acceptType = temp;
        }
        else if(arg == "Accept-Language:")
        {
            _acceptLang = temp;
        }
        else if(arg == "Accept-Encoding:")
        {
           _acceptEnc = temp;
        }
        else if(arg == "Connection:")
        {
            _connection = temp;
        }
        else if(arg == "Range:")
        {
            _range = temp;
        }
        else if(arg == "If-Range:")
        {
            _ifRange = temp;
        }
        else if(arg == "Cache-Control:")
        {
            _cacheControl = temp;
        }
        else if (arg == "If-Modified-Since:") {
            _modifiedSice = QDateTime::fromString(temp,"ddd',' dd MMM yyyy hh:mm:ss 'GMT'");
            _modifiedSice.setTimeZone(QTimeZone::utc());
        }
        else if (arg == "Upgrade-Insecure-Requests") {
            _upgradeInsecure = temp == "0" ? false : true;
        }
    }
    return true;
}


QString HTTPObject::getGetAddress() const
{
    return _getAddress;
}

QString HTTPObject::getVersionString(bool show09) const
{
    return shortToStringVersion(_HTTPVersion, show09);
}
unsigned short HTTPObject::getVersion() const
{
    return _HTTPVersion;
}
QByteArray HTTPObject::makeMsg() const
{
    return makeMsg(*this);
}
void HTTPObject::setEncoding(Encoding enc)
{
    _sendEnc = enc;
}

HTTPObject::Encoding * HTTPObject::getAcceptedEncodings(Encoding * encodings, int maxSize)
{
    QStringList allEnc = _acceptEnc.split(",");
    for(int i = 0; i < allEnc.length(); i++)
    {
        for(int k = i+1; i < allEnc.length(); k++)
        {
            int kData = allEnc[k].length() > 1 ? allEnc[k].split(";q=")[1].toInt() : 0;
            int iData = allEnc[i].length() > 1 ? allEnc[i].split(";q=")[1].toInt() : 0;
            if(iData < kData)
            {
                allEnc.move(k,i);
            }
        }
    }
    maxSize  = maxSize > allEnc.length() ? allEnc.length() : maxSize;
    for(int i = 0; i < maxSize; i++)
    {
        encodings[i] = encodingEnum(allEnc[i].split(";")[0]);
    }
    return encodings;
}


void HTTPObject::setKeepAlive(bool set)
{
    _keepAlive = set;
}

QByteArray HTTPObject::makeMsg(const HTTPObject & obj)
{
    if(obj._HTTPVersion == 2304 || !obj._serverHeaders) // Version 0.9, 0000 1001 = 9, 0000 0000 = 0, 0000 1001 0000 0000 = 2304
    {
        return obj._msg;
    }
    else
    {
        QString a1("%1");
        QByteArray r;
        QDataStream tempMsg(&r,QIODevice::WriteOnly);
        tempMsg.writeRawData("HTTP",4);
        tempMsg.writeRawData("/",1);
        tempMsg.writeRawData(obj.getVersionString().toUtf8(),obj.getVersionString().length());
        tempMsg.writeRawData(" ",1);
        tempMsg.writeRawData(intToStringCode(obj._code).toUtf8(),intToStringCode(obj._code).length());
        tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());

        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & HTTPCahced || obj._serverHeaders & HTTPRedirict || obj._serverHeaders & server)
        {
            tempMsg.writeRawData(serverString.arg(serverName).toUtf8(),serverString.arg(serverName).length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
        }
        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & HTTPCahced || obj._serverHeaders & HTTPRedirict || obj._serverHeaders & date)
        {
            tempMsg.writeRawData(dateString.toUtf8(),dateString.length());
            tempMsg.writeRawData(" ",1);
            tempMsg.writeRawData(getHTTPDateFormat().toUtf8(),getHTTPDateFormat().length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
        }
        if((obj._HTTPVersion == 257 &&(obj._serverHeaders & HTTPDefault || obj._serverHeaders & HTTPCahced)) || obj._serverHeaders & keepAlive) // Version 1.1, 0000 0001 = 1, 0000 0001 = 1, 0001 0001 = 257
        {
            QString temp(connection.arg(obj._keepAlive ? "keep-alive" : "close"));
            tempMsg.writeRawData(temp.toUtf8(),temp.length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
            if(obj._keepAlive)
            {
                QString temp2(keepAliveString.arg(obj._KAtimeout).arg(obj._KAmax));
                tempMsg.writeRawData(temp2.toUtf8(), temp2.length());
                tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
            }
        }
        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & contentType)
        {

            tempMsg.writeRawData(contentTypeString.toUtf8(),contentTypeString.length());
            tempMsg.writeRawData(" ",1);
            tempMsg.writeRawData(obj._DataType.toUtf8(),obj._DataType.length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
        }

        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & modyfied)
        {
            tempMsg.writeRawData(modify.toUtf8(),modify.length());
            tempMsg.writeRawData(" ",1);
            tempMsg.writeRawData(getHTTPDateFormat(obj._modified.isValid() ? &(obj._modified) : nullptr).toUtf8(),getHTTPDateFormat(obj._modified.isValid() ? &(obj._modified) : nullptr).length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
        }

        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & length)
        {
            tempMsg.writeRawData(lengthString.toUtf8(),lengthString.length());
            tempMsg.writeRawData(" ",1);
            tempMsg.writeRawData(a1.arg(obj._msg.length()).toUtf8(),a1.arg(obj._msg.length()).length());
            tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());
        }

        if(obj._serverHeaders & HTTPRedirict || obj._serverHeaders & location)
        {
            tempMsg.writeRawData(locationString.arg(obj._location).toUtf8(),locationString.arg(obj._location).length());
        }

        if(obj._serverHeaders & HSTS)
        {
            tempMsg.writeRawData(hstsString.arg(obj._HSTSTime).toUtf8(),hstsString.arg(obj._HSTSTime).length());
        }
        tempMsg.writeRawData(httpEndl.toUtf8(),httpEndl.length());

        if(obj._serverHeaders & HTTPDefault || obj._serverHeaders & message)
        {
            tempMsg.writeRawData(obj._msg,obj._msg.length());
        }
        return r;
    }

}

void HTTPObject::setModified(QDateTime & modified)
{
    _modified = modified;
}

QString HTTPObject::getHTTPDateFormat(const QDateTime * date, const QTimeZone * timezoneOffset)
{
    QDateTime dateTime;
    if(date == nullptr)
        dateTime = QDateTime::currentDateTime();
    else
        dateTime = *date;
    if(timezoneOffset == nullptr)
        dateTime=dateTime.toTimeZone(QTimeZone::utc());
    else
        dateTime = dateTime.toTimeZone(*timezoneOffset);
    return dateTime.toString("ddd',' dd MMM yyyy hh:mm:ss 'GMT'");
}

QString HTTPObject::encodingString(HTTPObject::Encoding enc)
{
    switch (enc)
    {
    case HTTPObject::noEncoding:
        return "identity";
    case HTTPObject::gzip:
        return "gzip";
    case HTTPObject::LZW:
        return "compress";
    case HTTPObject::deflate:
        return "deflate";
    case HTTPObject::Brotli:
        return "br";
    case HTTPObject::noPreference:
        return "";
    }
    return "";
}

HTTPObject::Encoding HTTPObject::encodingEnum(QString enc)
{
    if(enc == "identity")
        return HTTPObject::noEncoding;
    else if(enc == "gzip")
        return HTTPObject::gzip;
    else if(enc == "compress")
        return  HTTPObject::LZW;
    else if(enc =="deflate")
        return HTTPObject::deflate;
    else if(enc == "br")
        return HTTPObject::Brotli;
    return HTTPObject::noPreference;
}

QString HTTPObject::shortToStringVersion(short version, bool show09)
{
    char* versions = reinterpret_cast<char *>(&version);
    if(!show09 && versions[0]==0 && versions[1]==9)
    {
        return "";
    }
    QString verString(versions[0]+48);
    verString.append('.');
    verString.append(versions[1]+48);
    return verString;
}

void HTTPObject::setServerHeaders(int head)
{
    _serverHeaders = head;
}
void HTTPObject::setServerHeaders(headers head)
{
    _serverHeaders = static_cast<int>(head);
}

QDateTime HTTPObject::getModified() const
{
    return _modified;
}
QDateTime HTTPObject::getIfModifiedSince() const
{
    return _modifiedSice;
}

void HTTPObject::setMsg(QString msg)
{
    _msg = msg.toUtf8();
}

void HTTPObject::setMsg(QByteArray msg)
{
    _msg = msg;
}

void HTTPObject::setDataType(QString dataType)
{
    _DataType = dataType;
}
void HTTPObject::setCode(int code)
{
    switch (code)
    {
    case 100:
    case 101:
    case 102:
    case 103:
    case 200:
    case 201:
    case 202:
    case 203:
    case 204:
    case 205:
    case 206:
    case 207:
    case 208:
    case 226:
    case 300:
    case 301:
    case 302:
    case 303:
    case 304:
    case 305:
    case 306:
    case 307:
    case 308:
    case 400:
    case 401:
    case 402:
    case 403:
    case 404:
    case 405:
    case 406:
    case 407:
    case 408:
    case 409:
    case 410:
    case 411:
    case 412:
    case 413:
    case 414:
    case 415:
    case 416:
    case 417:
    case 418:
    case 421:
    case 422:
    case 423:
    case 424:
    case 425:
    case 426:
    case 428:
    case 429:
    case 431:
    case 451:
    case 500:
    case 501:
    case 502:
    case 503:
    case 504:
    case 505:
    case 506:
    case 507:
    case 508:
    case 510:
    case 511:
        _code = code;
        break;
    default:
        _code = 500; //Internal server error;
    }
}
int HTTPObject::getCode() const
{
    return _code;
}

QString HTTPObject::intToStringCode(int code)
{
    switch (code)
    {
/*Information responses*/

    case 100:
        return "100 Continue";
        //This interim response indicates that everything so far is OK and that the client should continue the request, or ignore the response if the request is already finished.
    case 101:
        return "101 Switching Protocol";
        //This code is sent in response to an Upgrade request header from the client, and indicates the protocol the server is switching to.
    case 102:
        return "102 Processing (WebDAV)";
        //This code indicates that the server has received and is processing the request, but no response is available yet.
    case 103:
        return "103 Early Hints";
        //This status code is primarily intended to be used with the Link header, letting the user agent start preloading resources while the server prepares a response.

/*Successful responses*/

    case 200:
        return "200 OK";
        /*The request has succeeded. The meaning of the success depends on the HTTP method:

        GET: The resource has been fetched and is transmitted in the message body.
        HEAD: The entity headers are in the message body.
        PUT or POST: The resource describing the result of the action is transmitted in the message body.
        TRACE: The message body contains the request message as received by the server*/

    case 201:
        return "201 Created";
        //The request has succeeded and a new resource has been created as a result. This is typically the response sent after POST requests, or some PUT requests.
    case 202:
        return "202 Accepted";
        //The request has been received but not yet acted upon. It is noncommittal, since there is no way in HTTP to later send an asynchronous response indicating the outcome of the request. It is intended for cases where another process or server handles the request, or for batch processing.
    case 203:
        return "203 Non-Authoritative Information";
        //This response code means the returned meta-information is not exactly the same as is available from the origin server, but is collected from a local or a third-party copy. This is mostly used for mirrors or backups of another resource. Except for that specific case, the "200 OK" response is preferred to this status.
    case 204:
        return "204 No Content";
        //There is no content to send for this request, but the headers may be useful. The user-agent may update its cached headers for this resource with the new ones.
    case 205:
        return "205 Reset Content";
        //Tells the user-agent to reset the document which sent this request.
    case 206:
        return "206 Partial Content";
        //This response code is used when the Range header is sent from the client to request only part of a resource.
    case 207:
        return "207 Multi-Status (WebDAV)";
        //Conveys information about multiple resources, for situations where multiple status codes might be appropriate.
    case 208:
        return "208 Already Reported (WebDAV)";
        //Used inside a <dav:propstat> response element to avoid repeatedly enumerating the internal members of multiple bindings to the same collection.
    case 226:
        return "226 IM Used (HTTP Delta encoding)";
        //The server has fulfilled a GET request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.

/*Redirection messages*/

    case 300:
        return "300 Multiple Choice";
        //The request has more than one possible response. The user-agent or user should choose one of them. (There is no standardized way of choosing one of the responses, but HTML links to the possibilities are recommended so the user can pick.)
    case 301:
        return "301 Moved Permanently";
        //The URL of the requested resource has been changed permanently. The new URL is given in the response.
    case 302:
        return "302 Found";
        //This response code means that the URI of requested resource has been changed temporarily. Further changes in the URI might be made in the future. Therefore, this same URI should be used by the client in future requests.
    case 303:
        return "303 See Other";
        //The server sent this response to direct the client to get the requested resource at another URI with a GET request.
    case 304:
        return "304 Not Modified";
        //This is used for caching purposes. It tells the client that the response has not been modified, so the client can continue to use the same cached version of the response.
    case 305:
        return "305 Use Proxy";
        //Defined in a previous version of the HTTP specification to indicate that a requested response must be accessed by a proxy. It has been deprecated due to security concerns regarding in-band configuration of a proxy.
    case 306:
        return "306 unused";
        //This response code is no longer used; it is just reserved. It was used in a previous version of the HTTP/1.1 specification.
    case 307:
        return "307 Temporary Redirect";
        //The server sends this response to direct the client to get the requested resource at another URI with same method that was used in the prior request. This has the same semantics as the 302 Found HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request.
    case 308:
        return "308 Permanent Redirect";
        //This means that the resource is now permanently located at another URI, specified by the Location: HTTP Response header. This has the same semantics as the 301 Moved Permanently HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request.

/*Client error responses*/

    case 400:
        return "400 Bad Request";
        //The server could not understand the request due to invalid syntax.
    case 401:
        return "401 Unauthorized";
        //Although the HTTP standard specifies "unauthorized", semantically this response means "unauthenticated". That is, the client must authenticate itself to get the requested response.
    case 402:
        return "402 Payment Required ";
        //This response code is reserved for future use. The initial aim for creating this code was using it for digital payment systems, however this status code is used very rarely and no standard convention exists.
    case 403:
        return "403 Forbidden";
        //The client does not have access rights to the content; that is, it is unauthorized, so the server is refusing to give the requested resource. Unlike 401, the client's identity is known to the server.
    case 404:
        return "404 Not Found";
        //The server can not find the requested resource. In the browser, this means the URL is not recognized. In an API, this can also mean that the endpoint is valid but the resource itself does not exist. Servers may also send this response instead of 403 to hide the existence of a resource from an unauthorized client. This response code is probably the most famous one due to its frequent occurrence on the web.
    case 405:
        return "405 Method Not Allowed";
        //The request method is known by the server but has been disabled and cannot be used. For example, an API may forbid DELETE-ing a resource. The two mandatory methods, GET and HEAD, must never be disabled and should not return this error code.
    case 406:
        return "406 Not Acceptable";
        //This response is sent when the web server, after performing server-driven content negotiation, doesn't find any content that conforms to the criteria given by the user agent.
    case 407:
        return "407 Proxy Authentication Required";
        //This is similar to 401 but authentication is needed to be done by a proxy.
    case 408:
        return "408 Request Timeout";
        //This response is sent on an idle connection by some servers, even without any previous request by the client. It means that the server would like to shut down this unused connection. This response is used much more since some browsers, like Chrome, Firefox 27+, or IE9, use HTTP pre-connection mechanisms to speed up surfing. Also note that some servers merely shut down the connection without sending this message.
    case 409:
        return "409 Conflict";
        //This response is sent when a request conflicts with the current state of the server.
    case 410:
        return "410 Gone";
        //This response is sent when the requested content has been permanently deleted from server, with no forwarding address. Clients are expected to remove their caches and links to the resource. The HTTP specification intends this status code to be used for "limited-time, promotional services". APIs should not feel compelled to indicate resources that have been deleted with this status code.
    case 411:
        return "411 Length Required";
        //Server rejected the request because the Content-Length header field is not defined and the server requires it.
    case 412:
        return "412 Precondition Failed";
        //The client has indicated preconditions in its headers which the server does not meet.
    case 413:
        return "413 Payload Too Large";
        //Request entity is larger than limits defined by server; the server might close the connection or return an Retry-After header field.
    case 414:
        return "414 URI Too Long";
        //The URI requested by the client is longer than the server is willing to interpret.
    case 415:
        return "415 Unsupported Media Type";
        //The media format of the requested data is not supported by the server, so the server is rejecting the request.
    case 416:
        return "416 Range Not Satisfiable";
        //The range specified by the Range header field in the request can't be fulfilled; it's possible that the range is outside the size of the target URI's data.
    case 417:
        return "417 Expectation Failed";
        //This response code means the expectation indicated by the Expect request header field can't be met by the server.
    case 418:
        return "418 I'm a teapot";
        //The server refuses the attempt to brew coffee with a teapot.
    case 421:
        return "421 Misdirected Request";
        //The request was directed at a server that is not able to produce a response. This can be sent by a server that is not configured to produce responses for the combination of scheme and authority that are included in the request URI.
    case 422:
        return "422 Unprocessable Entity (WebDAV)";
        //The request was well-formed but was unable to be followed due to semantic errors.
    case 423:
        return "423 Locked (WebDAV)";
        //The resource that is being accessed is locked.
    case 424:
        return "424 Failed Dependency (WebDAV)";
        //The request failed due to failure of a previous request.
    case 425:
        return "425 Too Early";
        //Indicates that the server is unwilling to risk processing a request that might be replayed.
    case 426:
        return "426 Upgrade Required";
        //The server refuses to perform the request using the current protocol but might be willing to do so after the client upgrades to a different protocol. The server sends an Upgrade header in a 426 response to indicate the required protocol(s).
    case 428:
        return "428 Precondition Required";
        //The origin server requires the request to be conditional. This response is intended to prevent the 'lost update' problem, where a client GETs a resource's state, modifies it, and PUTs it back to the server, when meanwhile a third party has modified the state on the server, leading to a conflict.
    case 429:
        return "429 Too Many Requests";
        //The user has sent too many requests in a given amount of time ("rate limiting").
    case 431:
        return "431 Request Header Fields Too Large";
        //The server is unwilling to process the request because its header fields are too large. The request may be resubmitted after reducing the size of the request header fields.
    case 451:
        return "451 Unavailable For Legal Reasons";
        //The user-agent requested a resource that cannot legally be provided, such as a web page censored by a government.

/*Server error responses*/

    case 500:
        return "500 Internal Server Error";
        //The server has encountered a situation it doesn't know how to handle.
    case 501:
        return "501 Not Implemented";
        //The request method is not supported by the server and cannot be handled. The only methods that servers are required to support (and therefore that must not return this code) are GET and HEAD.
    case 502:
        return "502 Bad Gateway";
        //This error response means that the server, while working as a gateway to get a response needed to handle the request, got an invalid response.
    case 503:
        return "503 Service Unavailable";
        //The server is not ready to handle the request. Common causes are a server that is down for maintenance or that is overloaded. Note that together with this response, a user-friendly page explaining the problem should be sent. This responses should be used for temporary conditions and the Retry-After: HTTP header should, if possible, contain the estimated time before the recovery of the service. The webmaster must also take care about the caching-related headers that are sent along with this response, as these temporary condition responses should usually not be cached.
    case 504:
        return "504 Gateway Timeout";
        //This error response is given when the server is acting as a gateway and cannot get a response in time.
    case 505:
        return "505 HTTP Version Not Supported";
        //The HTTP version used in the request is not supported by the server.
    case 506:
        return "506 Variant Also Negotiates";
        //The server has an internal configuration error: the chosen variant resource is configured to engage in transparent content negotiation itself, and is therefore not a proper end point in the negotiation process.
    case 507:
        return "507 Insufficient Storage (WebDAV)";
        //The method could not be performed on the resource because the server is unable to store the representation needed to successfully complete the request.
    case 508:
        return "508 Loop Detected (WebDAV)";
        //The server detected an infinite loop while processing the request.
    case 510:
        return "510 Not Extended";
        // Further extensions to the request are required for the server to fulfil it.
    case 511:
        return "511 Network Authentication Required";
        //The 511 status code indicates that the client needs to authenticate to gain network access. 
    default:
        return "500 Internal Server Error";
    }
}
#ifdef Q_OS_Win
QString HTTPObject::serverName("QT HTTP-server (Windows)");
#elifdef Q_OS_LINUX
QString HTTPObject::serverName("QT HTTP-server (Unix/Linux)");
#elifdef Q_OS_UNIX
QString HTTPObject::serverName("QT HTTP-server (Unix)");
#elifdef Q_OS_MAC
QString HTTPObject::serverName("QT HTTP-server (Unix/Mac)");
#else
QString HTTPObject::serverName("QT HTTP-server");
#endif
const QString HTTPObject::httpEndl("\r\n");
const QString HTTPObject::http("HTTP");
const QString HTTPObject::dateString("Date:");
const QString HTTPObject::contentTypeString("Content-Type:");
const QString HTTPObject::lengthString("Content-Length:");
const QString HTTPObject::modify("Last-Modified:");
const QString HTTPObject::serverString("Server: %1");
const QString HTTPObject::connection("Connection: %1");
const QString HTTPObject::keepAliveString("Keep-Alive: timeout=%1, max=%2");
const QString HTTPObject::locationString("Location: %1\r\n");

const QString HTTPObject::hstsString("Strict-Transport-Security: max-age=%1\r\n");
