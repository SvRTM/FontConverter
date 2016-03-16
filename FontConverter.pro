#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T14:17:18
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -mno-mmx \
       -mno-sse2 \
       -mno-sse3 \
       -mno-ssse3 \
       -mno-sse4.1 \
       -mno-sse4.2 \
       -mno-sse4 \
       -mno-avx \
       -mno-avx2 \
       -mno-avx512f \
       -mno-avx512pf \
       -mno-avx512er \
       -mno-avx512cd \
       -mno-sha \
       -mno-aes \
       -mno-pclmul \
        -mno-fsgsbase \
       -mno-rdrnd \
       -mno-f16c \
       -mno-fma \
       -mno-prefetchwt1 \
       -mno-sse4a \
       -mno-fma4 \
       -mno-xop \
       -mno-lwp \
       -mno-3dnow \
       -mno-popcnt \
       -mno-abm \
       -mno-bmi \
       -mno-bmi2 \
       -mno-lzcnt \
       -mno-tbm

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FontConverter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fontexport.cpp \
    symboltableitem.cpp \
    symboltablemodel.cpp \
    symboltableview.cpp \
    exportdialog.cpp

HEADERS  += mainwindow.h \
    fontexport.h \
    symboltableitem.h \
    symboltablemodel.h \
    symboltableview.h \
    exportdialog.h

FORMS    += mainwindow.ui \
    export.ui
