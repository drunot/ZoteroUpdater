#include "httplog.h"
#include <iostream>
#include <QDateTime>




httpLog::httpLog(QString path, bool echo, QObject *parent) : QObject(parent), _echo(echo)
{
    _file = new QFile(path);
    if(path == "")
    {
        _open = false;
    }
    _allLogs.append(this);
}

httpLog::~httpLog()
{
    _file->close();
    delete _file;
    _allLogs.removeOne(this);
}

bool httpLog::addString(QString msg)
{
    if(_open && _file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QString temp(createLogString(msg));
        _file->write(temp.toUtf8());
        if(_echo)
        {
            std::cout << temp.toStdString();
        }
        _file->close();
        return true;
    }
    else if(_echo)
    {
        std::cout << createLogString(msg).toStdString();
    }
    return false;
}

bool httpLog::addErrorString(QString msg)
{
    if(_open && _file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QString temp(createLogErrorString(msg));
        _file->write(temp.toUtf8());
        if(_echo)
        {
            std::cout << temp.toStdString();
        }
        _file->close();
        return true;
    }
    else if(_echo)
    {
        std::cout << createLogString(msg).toStdString();
    }
    return false;
}


QString httpLog::createLogString(const QString msg)
{
    return QDateTime::currentDateTime().toString("'['dd-MM-yyyy HH:mm:ss:zzz'] %1'").arg(msg).append('\n');
}

QString httpLog::createLogErrorString(const QString msg)
{
    return QDateTime::currentDateTime().toString("'[ERROR]['dd-MM-yyyy HH:mm:ss:zzz'] %1'").arg(msg).append('\n');
}

QList<httpLog *> httpLog::_allLogs;



void httpLog::addToLog(const QString &s)
{
    foreach (httpLog * log, _allLogs) {
        log->addString(s);
    }
}
void httpLog::addErrorToLog(const QString &s)
{
    foreach (httpLog * log, _allLogs) {
        log->addErrorString(s);
    }
}
void httpLog::addConnected(const QString &ip)
{
    QString temp("New connection: %1");
    addToLog(temp.arg(ip));
}
void httpLog::addDisconnected(const QString &ip)
{
    QString temp("Connection ended: %1");
    addToLog(temp.arg(ip));
}
void httpLog::addhttpRequest(const HTTPObject &obj, const QString &ip)
{
    QString temp("From %1. Get adress: %2 Http version: %3");
    addToLog(temp.arg(ip, obj.getGetAddress(), obj.getVersionString(true)));
}
