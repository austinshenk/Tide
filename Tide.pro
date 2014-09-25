#-------------------------------------------------
#
# Project created by QtCreator 2014-09-10T22:04:56
#
#-------------------------------------------------

QT       += core gui
QT       += uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tide
TEMPLATE = app


SOURCES += main.cpp\
        tide.cpp \
    filecontroller.cpp \
    tabwidget.cpp \
    textedit.cpp

HEADERS  += tide.h \
    filecontroller.h \
    tabwidget.h \
    textedit.h

FORMS    += tide.ui
