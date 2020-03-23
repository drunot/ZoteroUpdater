#include <QCoreApplication>
#include "httpserver.h"
#include "httpsserver.h"
#include "httpfilerule.h"
#include "httppathrule.h"
#include "httplog.h"
#include <iostream>
#include "httpfileoperations.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    httpLog log("log.log");
    HTTPFileRule pngRule("png", HTTPFileOperations::loadImage);
    HTTPFileRule jpgRule("jpg", HTTPFileOperations::loadImage);
    HTTPFileRule jpegRule("jepg", HTTPFileOperations::loadImage);
    HTTPFileRule gifRule("gif", HTTPFileOperations::loadImage);
    HTTPPathRule rootIndexRule("/", HTTPFileOperations::loadIndex);
    HTTPServer s;
    httpLog::addToLog("Server started. Version 1.0");
    if(!s.listen(QHostAddress::Any, 80))
    {
        httpLog::addErrorToLog("HTTP could not start");
    }
    else
    {
        httpLog::addToLog("HTTP started");
    }
    HTTPSServer ss;
    if(!ss.listen(QHostAddress::Any, 443))
    {
        httpLog::addErrorToLog("HTTPS could not start");
    }
    else
    {
        httpLog::addToLog("HTTPS started");
    }
    return a.exec();
}
