# -------------------------------------------------
# Project created by QtCreator 2012-08-01T21:16:15
# -------------------------------------------------
QT += core
QT -= gui
TARGET = opecv-qr
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_videostab
SOURCES += src/main.cpp \
    src/camera.cpp \
    src/window.cpp \
    src/display.cpp \
    src/filter.cpp \
    src/simplefilter.cpp \
    src/facefilter.cpp \
    src/qrpatternfilter.cpp \
    src/video.cpp
HEADERS += src/source.h \
    src/camera.h \
    src/window.h \
    src/display.h \
    src/filter.h \
    src/simplefilter.h \
    src/constants.h \
    src/facefilter.h \
    src/qrpatternfilter.h \
    src/video.h
