

#include"ZoteroUpdater.h"
#include <QProcess>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <iostream>
#include <QTextStream>
#include <string>
#include <sstream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomText>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
HTTPObject * ZoteroUpdater(HTTPObject * obj)
{
    QStringList list = obj->getGetAddress().split("/");
    if(list.length() < 3)
    {
        obj->setMsg(c404page);
        obj->setCode(404);
        return obj;
    }
    QFile file("userid.xml");
    file.open(QIODevice::ReadOnly);
    QTextStream s1(&file);
    QString test(s1.readAll());
    QDomDocument userIdDoc;
    userIdDoc.setContent(test);
    QDomNodeList users = userIdDoc.elementsByTagName("user");
    QString userId = "";
    for(int i = 0; i < users.length(); i++)
    {
        if(users.at(i).namedItem("name").firstChild().nodeValue() == list[1])
        {
            userId = users.at(i).namedItem("id").firstChild().nodeValue();
        }
    }
    if(userId.length() < 1)
    {
        obj->setMsg(c404user);
        obj->setCode(404);
        return obj;
    }
    QProcess getProc;
    QString curl("curl");
    QString group("https://api.zotero.org/users/");
    group.append(userId).append("/groups");
    QStringList idParms;
    idParms.append(group);
    getProc.start(curl, idParms);
    if(!getProc.waitForFinished(2000))
    {
        obj->setMsg(c503);
        obj->setCode(503);
        return obj;
    }
    QJsonDocument idDoc = QJsonDocument::fromJson(QString(getProc.readAllStandardOutput()).toUtf8());
    int id = 1;
    foreach (QJsonValue val, idDoc.array()) {
        if((val["data"])["name"].toString().toLower() == list[2].replace(".bib","").toLower())
        {
            id = val["id"].toInt();
            break;
        }
    }
    if(id == 1)
    {
        obj->setMsg(c404group);
        obj->setCode(404);
        return obj;
    }
    QString bibTex("https://api.zotero.org/groups/");
    bibTex.append(QString("%1").arg(id)).append("/items?content=bibtex&style=apa");
    QStringList bibTexParms;
    bibTexParms.append(bibTex);
    getProc.start(curl, bibTexParms);
    if(!getProc.waitForFinished(2000))
    {
        obj->setMsg(c503);
        obj->setCode(503);
        return obj;
    }
    QString output(getProc.readAllStandardOutput());
    //std::cout << output.toStdString();
    QDomDocument xmlBibTex;
    xmlBibTex.setContent(output);
    QDateTime updated(QDateTime::fromString(xmlBibTex.namedItem("feed").namedItem("updated").firstChild().nodeValue(),"yyyy-MM-dd'T'HH:mm:ss'Z'"));
    QDomNodeList xmlContent = xmlBibTex.elementsByTagName("content");
    QString bibDoc;
    for(int i = 0; i < xmlContent.length(); i++)
    {
        bibDoc.append(xmlContent.at(i).firstChild().nodeValue()).append("\n\n");
    }
    obj->setDataType("text/plain; charset=UTF-8");
    obj->setMsg(bibDoc);
    obj->setCode(200);
    obj->setModified(updated);
    return obj;
}
