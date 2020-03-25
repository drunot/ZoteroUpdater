#ifndef WEBTHREAD_H
#define WEBTHREAD_H

#include <QObject>
#include "webworker.h"

class webThread: public QObject
{
    Q_OBJECT
public:
    webThread(QObject *parent = nullptr);
    ~webThread();
    QThread getThread(int index);
    void terminateAll();
    void quitAll();
    int getNumberOfThreads();
private slots:
    void removeFromList(QObject * obj);
protected:
    void addAtEnd(qintptr socketDescriptor, bool sequre);
    QList<webWorker *> _allThreads;
};

#endif // WEBTHREAD_H
