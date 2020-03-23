#ifndef HTTPSWORKER_H
#define HTTPSWORKER_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QFile>
#include "httpobject.h"
#include "httpfilerule.h"
#include "httppathrule.h"
#include "httpfileoperations.h"
#include "httplog.h"
#include "webworker.h"
#include <QSslConfiguration>
#include <QSslKey>

class httpsWorker : public webWorker
{
    Q_OBJECT
public:
    explicit httpsWorker(qintptr socketDescriptor, QObject *parent = nullptr);
    void run() override
    {
        _workSocket = new QSslSocket;
        QSslSocket * _sslSocket = reinterpret_cast<QSslSocket * >(_workSocket);
        QByteArray key;
        QFile KeyFile("host.key");
        if(KeyFile.open(QIODevice::ReadOnly))
        {
            key = KeyFile.readAll();
            KeyFile.close();
        }
        else
        {
            qDebug() << KeyFile.errorString();
        }

        QSslKey sslKey(key, QSsl::Rsa);
        _sslSocket->setPrivateKey(sslKey);


        // Load server ssl certificate from file
        QByteArray cert;
        QFile CertFile("host.cert");
        if(CertFile.open(QIODevice::ReadOnly))
        {
            cert = CertFile.readAll();
            CertFile.close();
        }
        else
        {
            qDebug() << CertFile.errorString();
        }

        QSslCertificate sslCert(cert);
        _sslSocket->setLocalCertificate(sslCert);

        QSslConfiguration cfg = _sslSocket->sslConfiguration();
        cfg.caCertificates();

        if (!_sslSocket->setSocketDescriptor(_socketDescriptor))
        {
            httpLog::addErrorToLog("Couldn't set socket descriptor");
            return;
        }
        _ip_addr = _sslSocket->peerAddress().toString();

        emit connectedTo(_ip_addr);
        _sslSocket->startServerEncryption();

        while(_sslSocket->waitForReadyRead(3000))
        {
            if (!_sslSocket->isEncrypted())
            {
                httpLog::addErrorToLog("Socket not encrypted");
                return;
            }
            HTTPObject httpObj( _sslSocket->read(1000));
            emit gotHeader(httpObj, _ip_addr);
            if(!HTTPPathRule::runMatch(&httpObj) && !HTTPFileRule::runMatch(&httpObj))
            {
                HTTPFileOperations::FileHTML(&httpObj);
            }
            if(httpObj.getCode() == 200)
            {
                httpObj.setServerHeaders(HTTPObject::HTTPSDefault);
            }
            _sslSocket->write(httpObj.makeMsg());
            _sslSocket->flush();
            _sslSocket->waitForBytesWritten(3000);
        }
        _sslSocket->close();
    }
};

#endif // HTTPSWORKER_H
