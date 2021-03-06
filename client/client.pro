#Author : CONGILIO Luigi    CONSTANS Victor
#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T11:33:46
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mediacontrol.cpp \
    volumewidget.cpp \
    guiserver.cpp \
    myqslider.cpp

HEADERS  += mainwindow.h \
    mediacontrol.h \
    volumewidget.h \
    guiserver.h \
    myqslider.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressource.qrc

TRANSLATIONS =  client_fr.ts \
                client_en.ts \
                client_it.ts
