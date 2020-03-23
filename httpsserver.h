#ifndef HTTPSSERVER_H
#define HTTPSSERVER_H
#include <QtNetwork/QTcpServer>
#include "httpsthread.h"

class HTTPSServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HTTPSServer(QObject *parent = nullptr);

private:
    HttpsThread _theads;

protected:
    virtual void incomingConnection(qintptr socketDescriptor)
    {
        _theads.append(socketDescriptor);
    }
};

#endif // HTTPSSERVER_H
