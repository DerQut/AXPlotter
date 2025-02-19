#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T22:48:27
#
#-------------------------------------------------

QT      += core gui
CONFIG  += c++11

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
    detailview.cpp \
    editorview.cpp \
    axsyntaxhighlighter.cpp \
    linenumbersview.cpp

HEADERS  += mainwindow.h \
    themefile.h \
    contentview.h \
    axtoolbar.h \
    sidebarview.h \
    axmenubar.h \
    detailview.h \
    editorview.h \
    axsyntaxhighlighter.h \
    linenumbersview.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../Documents/Kalibracja.txt
