#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T22:03:03
#
#-------------------------------------------------

QT       += core gui

TARGET = radiotest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../../inc/bt_types.h \
    ../../inc/bt_if.h

FORMS    += mainwindow.ui

LIBS += -lbt_test
