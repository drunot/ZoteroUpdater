#include "httpserver.h"
#include <QTcpSocket>
#include "httpobject.h"
#include <QDateTime>
#include <iostream>
#include "httpfilerule.h"
#include "httppathrule.h"
#include "httplog.h"
#include <QFileInfo>
#include <QDebug>

HTTPServer::HTTPServer(QObject *parent) : QTcpServer(parent)
{
}
