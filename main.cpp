#include "mainwindow.h"
#include "contentview.h"
#include "axmenubar.h"
#include <QApplication>
#include <QMenuBar>

// Created by Marcel Cholodecki on 2024.12.24

int main(int argc, char *argv[])
{
    QApplication mainApp(argc, argv);
    MainWindow* mainWindow = new MainWindow();
    mainWindow->setWindowTitle("AX Plotter");

    AXMenuBar* menubar = new AXMenuBar(mainWindow);

    mainWindow->setMenuBar(menubar);

    mainWindow->setCentralWidget(new ContentView());

    mainWindow->show();

    return mainApp.exec();
}
