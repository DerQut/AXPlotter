#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T22:48:27
#
#-------------------------------------------------

QT      += core gui
CONFIG  += c++11
QMAKE_CXXFLAGS += -std=c++11

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
    recentfilesview.cpp \
    axinterpreter.cpp \
    recursiveReplaceRead.cpp \
    convertaxmtopy.cpp

HEADERS  += mainwindow.h \
    themefile.h \
    contentview.h \
    axtoolbar.h \
    sidebarview.h \
    axmenubar.h \
    detailview.h \
    editorview.h \
    axsyntaxhighlighter.h \
    recentfilesview.h \
    axinterpreter.h \
    recursiveReplaceRead.h \
    convertaxmtopy.h

OTHER_FILES += \
    ../Documents/Kalibracja.txt
