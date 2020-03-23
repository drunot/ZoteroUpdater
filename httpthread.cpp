#include "httpthread.h"

HttpThread::HttpThread(QObject *parent) : webThread(parent)
{
}

void HttpThread::append(qintptr socketDescriptor)
{
    addAtEnd(socketDescriptor, false);
}
