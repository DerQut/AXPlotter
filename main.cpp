#include "mainwindow.h"
#include "contentview.h"
#include <QApplication>
#include <QMenuBar>

// Created by Marcel Cholodecki on 2024.12.24

int main(int argc, char *argv[])
{
    QApplication mainApp(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("AX Plotter");

    QMenuBar* menubar = new QMenuBar(&mainWindow);

    menubar->addAction("File");

    mainWindow.setMenuBar(menubar);

    mainWindow.setCentralWidget(new ContentView());

    mainWindow.show();

    return mainApp.exec();
}
