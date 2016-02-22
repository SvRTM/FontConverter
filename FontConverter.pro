#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T14:17:18
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FontConverter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    tablemodel.cpp \
    tableitem.cpp \
    tableview.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    tablemodel.h \
    tableitem.h \
    tableview.h

FORMS    += mainwindow.ui
