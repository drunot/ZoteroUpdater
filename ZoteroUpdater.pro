QT -= gui

QT += xml network

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += static

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ZoteroUpdater.cpp \
        httpfileoperations.cpp \
        httpfilerule.cpp \
        httplog.cpp \
        httpobject.cpp \
        httppathrule.cpp \
        httpserver.cpp \
        httpsserver.cpp \
        httpsthread.cpp \
        httpsworker.cpp \
        httpthread.cpp \
        httpworker.cpp \
        main.cpp \
        webthread.cpp \
        webworker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ZoteroUpdater.h \
    httpfileoperations.h \
    httpfilerule.h \
    httplog.h \
    httpobject.h \
    httppathrule.h \
    httpserver.h \
    httpsserver.h \
    httpsthread.h \
    httpsworker.h \
    httpthread.h \
    httpworker.h \
    webthread.h \
    webworker.h
