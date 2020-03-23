#include "httpfileoperations.h"
#include <QFile>
#include <QFileInfo>

HTTPObject * HTTPFileOperations::FileHTML(HTTPObject * obj)
{
    QString filePath("WWW");
    filePath.append(obj->getGetAddress());
    if(!QFileInfo::exists(filePath))
    {
        if(!QFileInfo::exists(filePath.append(".html")))
        {
            obj->setCode(404);
            LoadCode(obj);
            return obj;
        }
    }
    QFile html(filePath);
    html.open(QFile::ReadOnly);
    QFileInfo htmlInfo(html);
    QDateTime lastMod(htmlInfo.lastModified());
    obj->setModified(lastMod);
    if(obj->getModified().isValid() && obj->getIfModifiedSince().isValid() && obj->getIfModifiedSince().msecsTo(obj->getModified())<1000 )
    {
        obj->setCode(304);
        obj->setServerHeaders(HTTPObject::HTTPCahced);
    }
    else
    {
        obj->setMsg(html.readAll());
        obj->setCode(200);
        obj->setDataType("text/html; charset=UTF-8");
    }
    return obj;
}



HTTPObject * HTTPFileOperations::LoadCode(HTTPObject * obj)
{
    QString path("CODES/%1.html");
    path = path.arg(obj->getCode());
    if(QFileInfo::exists(path))
    {
        QFile html(path);
        html.open(QFile::ReadOnly);
        QFileInfo htmlInfo(html);
        QDateTime lastMod(htmlInfo.lastModified());
        obj->setModified(lastMod);
        if(obj->getModified().isValid() && obj->getIfModifiedSince().isValid() && obj->getIfModifiedSince().msecsTo(obj->getModified())<1000 )
        {
            obj->setCode(304);
            obj->setServerHeaders(HTTPObject::HTTPCahced);
        }
        else
        {
            obj->setMsg(html.readAll());
            obj->setDataType("text/html; charset=UTF-8");
        }
    }
    else
    {
        QString msg("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>%1</title>\n\t</head>\n\t<body>\n\t\t<h1>Page return code: %1</h1>\n\t\t<p>The page returned with the code %1.</br>\n\tCheck if the address was misspelled or try agian.</p>\n\t</body>\n</html>");
        obj->setMsg(msg.arg(HTTPObject::intToStringCode(obj->getCode())));
        obj->setDataType("text/html; charset=UTF-8");
    }
    return obj;

}


HTTPObject * HTTPFileOperations::FileGeneral(HTTPObject *obj, QString dataType)
{
    QString filePath("WWW");
    filePath.append(obj->getGetAddress());
    if(QFileInfo::exists(filePath))
    {
        QFile f(filePath);
        f.open(QFile::ReadOnly);
        QFileInfo fInfo(f);
        QDateTime lastMod(fInfo.lastModified());
        obj->setModified(lastMod);
        if(obj->getModified().isValid() && obj->getIfModifiedSince().isValid() && obj->getIfModifiedSince().msecsTo(obj->getModified())<1000 )
        {
            obj->setCode(304);
            obj->setServerHeaders(HTTPObject::HTTPCahced);
        }
        else
        {
            obj->setMsg(f.readAll());
            obj->setCode(200);
            obj->setDataType(dataType);
        }
    }
    else
    {
        obj->setCode(404);
        LoadCode(obj);
    }
    return obj;
}

HTTPObject * HTTPFileOperations::loadIndex(HTTPObject * obj)
{
    if(QFileInfo::exists("WWW/index.html"))
    {
        QFile html("WWW/index.html");
        html.open(QFile::ReadOnly);
        obj->setMsg(html.readAll());
        obj->setCode(200);
        obj->setDataType("text/html; charset=UTF-8");
    }
    else
    {
        obj->setCode(404);
        LoadCode(obj);
    }
    return obj;
}


HTTPObject * HTTPFileOperations::loadImage(HTTPObject * obj)
{
    QString dataType("image/");
    dataType.append(obj->getGetAddress().split(".").last());
    FileGeneral(obj,dataType);
    return obj;
}
