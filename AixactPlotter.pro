#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T22:48:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AixactPlotter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    contentview.cpp \
    axmenubar.cpp \
    sidebarview.cpp \
    axtoolbar.cpp \
    themefile.cpp \
    sidebarentryview.cpp \
    fileentryview.cpp

HEADERS  += mainwindow.h \
    contentview.h \
    axmenubar.h \
    sidebarview.h \
    axtoolbar.h \
    themefile.h

FORMS    += mainwindow.ui
