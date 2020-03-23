#ifndef HTTPLOG_H
#define HTTPLOG_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>
#include <httpobject.h>

class httpLog : public QObject
{
    Q_OBJECT
public:
    explicit httpLog(QString path = "", bool echo = true, QObject *parent = nullptr);
    ~httpLog();
    bool addString(QString msg);
    bool addErrorString(QString msg);
signals:

public slots:
     static void addToLog(const QString &s);
     static void addErrorToLog(const QString &s);
     static void addConnected(const QString &ip);
     static void addDisconnected(const QString &ip);
     static void addhttpRequest(const HTTPObject &obj, const QString &ip);
private:
     static QString createLogString(const QString msg);
     static QString createLogErrorString(const QString msg);
     static QList<httpLog *> _allLogs;
     QFile * _file;
     bool _echo;
     bool _open;
};

#endif // HTTPLOG_H
