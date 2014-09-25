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
    tidetabwidget.cpp \
    tidetextedit.cpp

HEADERS  += tide.h \
    tidetabwidget.h \
    tidetextedit.h

FORMS    += tide.ui
