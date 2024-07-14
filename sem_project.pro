QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sem_project
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += mainwindow.ui

CONFIG += c++11
