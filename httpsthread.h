#ifndef HTTPSTHREAD_H
#define HTTPSTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "httpsworker.h"
#include "webthread.h"

class HttpsThread : public webThread
{
    Q_OBJECT
public:
    explicit HttpsThread(QObject *parent = nullptr);
    void append(qintptr socketDescriptor);
};

#endif // HTTPSTHREAD_H
