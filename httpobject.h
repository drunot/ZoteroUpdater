#ifndef HTTPOBJECT_H
#define HTTPOBJECT_H
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QDate>

class HTTPObject
{
public:
    enum Encoding{noEncoding = 0, gzip = 1, LZW = 2, deflate = 3, Brotli = 4, noPreference = 5};
    enum headers{HTTPNoHeader = 0x00,
                 HTTPDefault = 0x01,
                 HTTPRedirict = 0x02,
                 HTTPCahced = 0x04,
                 HSTS = 0x08,
                 HTTPSDefault = 0x09,
                 message=0x100,
                 modyfied = 0x200,
                 keepAlive = 0x400,
                 dataType = 0x800,
                 encoding = 0x1000,
                 location= 0x2000 ,
                 host = 0x4000,
                 userAgent = 0x8000,
                 acceptType = 0x10000,
                 acceptLang = 0x20000,
                 acceptEnc = 0x40000,
                 connectionType = 0x80000,
                 upgradeInsecure = 0x100000,
                 range = 0x200000,
                 if_range=0x400000,
                 clientCacheControl=0x800000,
                 server = 0x1000000,
                 length = 0x2000000,
                 date = 0x4000000,
                 contentType = 0x8000000};

    HTTPObject();
    HTTPObject(const QString phrase);
    bool parseHTTPString(const QString phrase);
    QString getGetAddress() const;
    QString getVersionString(bool show09 = false) const;
    unsigned short getVersion() const;
    QByteArray makeMsg() const;
    void setEncoding(Encoding enc);
    Encoding * getAcceptedEncodings(Encoding * encodings, int maxSize = 5);
    void setModified(QDateTime & modified);
    void setMsg(QString msg);
    void setMsg(QByteArray msg);
    void setDataType(QString dataType);
    void setCode(int code);
    int getCode() const;
    void setKeepAlive(bool set);
    QDateTime getModified() const;
    QDateTime getIfModifiedSince() const;
    void setServerHeaders(int head);
    void setServerHeaders(headers head);

    static QByteArray makeMsg(const HTTPObject & obj);
    static QString intToStringCode(int code);
    static QString shortToStringVersion(short version, bool show09 = false);
    static QString encodingString(Encoding enc);
    static Encoding encodingEnum(QString enc);
    static QString getHTTPDateFormat(const QDateTime * date=nullptr,const QTimeZone * timezoneOffset = nullptr);
private:
    //Client msg attributes
    QString _reqType;
    QString _getAddress = "";
    unsigned short _HTTPVersion;
    QString _host;
    QString _userAgent;
    QString _acceptType;
    QString _acceptLang;
    QString _acceptEnc;
    QString _range;
    QString _ifRange;
    bool _upgradeInsecure;
    int _clientHeaders = HTTPDefault;

    //Server msg attributes
    QString _sendEnc;
    QDateTime _modified;
    bool _keepAlive = true;
    int _KAtimeout = 3;
    int _KAmax = 1000;
    QByteArray _msg;
    QString _DataType = "text/html; charset=UTF-8";
    int _code = 200;
    QString _vary = "";
    QString _location;
    static QString serverName;
    int _HSTSTime = 31536000;
    int _serverHeaders = HTTPDefault;

    //Both msg attributes
    QString _cacheControl;
    QDateTime _modifiedSice;
    QString _connection;




    static const QString httpEndl;
    static const QString http;
    static const QString dateString;
    static const QString contentTypeString;
    static const QString lengthString;
    static const QString modify;
    static const QString serverString;
    static const QString connection;
    static const QString keepAliveString;
    static const QString locationString;
    static const QString hstsString;
};

#endif // HTTPOBJECT_H
