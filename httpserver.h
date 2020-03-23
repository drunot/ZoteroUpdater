#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <QtNetwork/QTcpServer>
#include "httpthread.h"

class HTTPServer : public QTcpServer
{
    Q_OBJECT
public:
    HTTPServer(QObject *parent = nullptr);

private:
    HttpThread _theads;

protected:
    virtual void incomingConnection(qintptr socketDescriptor)
    {
         _theads.append(socketDescriptor);
    }
};

#endif // HTTPSERVER_H
