#ifndef HTTPWORKER_H
#define HTTPWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include "httpobject.h"
#include "httpfilerule.h"
#include "httppathrule.h"
#include "httpfileoperations.h"
#include "webworker.h"

class httpWorker : public webWorker
{
    Q_OBJECT
public:
    explicit httpWorker(qintptr socketDescriptor, QObject *parent = nullptr);
    void run() override
    {
        _workSocket = new QTcpSocket;
        _workSocket->setSocketDescriptor(_socketDescriptor);
        _ip_addr = _workSocket->peerAddress().toString();
        emit connectedTo(_ip_addr);
        while(_workSocket->waitForReadyRead(3000))
        {
        HTTPObject httpObj( _workSocket->read(1000));
        emit gotHeader(httpObj, _ip_addr);
        if(!HTTPPathRule::runMatch(&httpObj) && !HTTPFileRule::runMatch(&httpObj))
        {
            HTTPFileOperations::FileHTML(&httpObj);
        }
        _workSocket->write(httpObj.makeMsg());
        _workSocket->flush();
        _workSocket->waitForBytesWritten(3000);
        }
        _workSocket->close();
    }
};

#endif // HTTPWORKER_H
