#ifndef WEBWORKER_H
#define WEBWORKER_H

#include <QThread>
#include <QAbstractSocket>
#include "httpobject.h"

class webWorker : public QThread
{
    Q_OBJECT
public:
    explicit webWorker(qintptr socketDescriptor, QObject *parent = nullptr);
    ~webWorker();
    void run() override = 0;
    QString getIP();

signals:
    void connectedTo(const QString &ip);
    void disconnectedFrom(const QString &ip);
    void gotHeader(const HTTPObject &obj, const QString &ip);
protected:
    QString _ip_addr;
    qintptr _socketDescriptor;
    QAbstractSocket * _workSocket;
};

#endif // WEBWORKER_H
