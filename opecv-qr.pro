#-------------------------------------------------
#
# Project created by QtCreator 2012-08-01T21:16:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = opecv-qr
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
LIBS    += -L/usr/local/lib\
        -lopencv_core -lopencv_highgui -lopencv_imgproc


SOURCES += main.cpp
