#include "httpsthread.h"
#include "httpsworker.h"
#include "httplog.h"
#include <QObject>

HttpsThread::HttpsThread(QObject *parent) : webThread(parent)
{
}

void HttpsThread::append(qintptr socketDescriptor)
{
    addAtEnd(socketDescriptor, true);
}
