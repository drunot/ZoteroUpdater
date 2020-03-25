#include "webthread.h"
#include "httpworker.h"
#include "httpsworker.h"
#include "httplog.h"

webThread::webThread(QObject * parent) : QObject(parent)
{

}

webThread::~webThread()
{
    quitAll();
}

void webThread::removeFromList(QObject * obj)
{
    if(obj != nullptr)
    {
    webWorker * temp = reinterpret_cast<webWorker *>(obj);
    _allThreads.removeOne(temp);
    }
}

void webThread::quitAll()
{
    foreach (webWorker * ptr, _allThreads)
    {
        ptr->quit();
        ptr->wait();
    }
}


void webThread::terminateAll()
{
    foreach (webWorker * ptr, _allThreads)
    {
        ptr->terminate();
    }
}
void webThread::addAtEnd(qintptr socketDescriptor, bool sequre)
{
    webWorker *worker;
    if(sequre)
    {
    worker = new httpsWorker(socketDescriptor);
    }
    else
    {
    worker = new httpWorker(socketDescriptor);
    }
    connect(worker, &httpWorker::finished, worker, &QObject::deleteLater);
    connect(worker, &httpWorker::destroyed, this, &webThread::removeFromList);
    connect(worker, &httpWorker::connectedTo, &httpLog::addConnected);
    connect(worker, &httpWorker::disconnectedFrom, &httpLog::addDisconnected);
    connect(worker, &httpWorker::gotHeader, &httpLog::addhttpRequest);
    _allThreads.append(worker);
    worker->start();
}
int webThread::getNumberOfThreads()
{
    return _allThreads.length();
}
