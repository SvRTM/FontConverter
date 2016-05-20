TEMPLATE = app
CONFIG += c++11

QT += core gui widgets

lessThan(QT_MAJOR_VERSION, 5): error(The Qt$$QT_MAJOR_VERSION is not supported!)

TARGET = FontConverter
CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_d)
    else: TARGET = $$join(TARGET,,,d)
}

VPATH += ./src
INCLUDEPATH += ./src

include(src/FontConverter.pri)
