#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T17:57:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0
QMAKE_TARGET_PRODUCT = ProcessMonitor
QMAKE_TARGET_COPYRIGHT = VladMigura


TARGET = ProcessMonitor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    cpuinfo.cpp \
    memoryinfo.cpp \
    updatethread.cpp \
    systeminfo.cpp

HEADERS += \
        mainwindow.h \
    cpuinfo.h \
    memoryinfo.h \
    updatethread.h \
    systeminfo.h

FORMS += \
        mainwindow.ui

LIBS += -lPsapi

RC_ICONS = icon.ico
