#Author : CONGILIO Luigi    CONSTANS Victor

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T18:23:27
#
#-------------------------------------------------

QT       += core network concurrent

QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    qmpvsocket.cpp \
    qguiclientsocket.cpp

HEADERS += \
    server.h \
    qmpvsocket.h \
    qguiclientsocket.h

INCLUDEPATH += /usr/local/include/taglib
LIBS += -L/usr/local/lib \
        -ltag \
        -lz
