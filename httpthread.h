#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "httpworker.h"
#include "webthread.h"

class HttpThread : public webThread
{
    Q_OBJECT
public:
    explicit HttpThread(QObject * parent = nullptr);
    void append(qintptr socketDescriptor);
};

#endif // HTTPTHREAD_H
