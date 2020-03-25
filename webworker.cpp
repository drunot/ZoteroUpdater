#include "webworker.h"

webWorker::webWorker(qintptr socketDescriptor, QObject *parent) : QThread(parent)
{
    _socketDescriptor = socketDescriptor;
}

webWorker::~webWorker() 
{
    emit disconnectedFrom(_ip_addr);
    delete _workSocket;
}

QString webWorker::getIP()
{
    return _workSocket->peerAddress().toString();
}
