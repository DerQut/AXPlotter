#include "mainwindow.h"
#include "contentview.h"
#include <QApplication>

// Created by Marcel Cholodecki on 2024.12.24

int main(int argc, char *argv[])
{
    QApplication mainApp(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("AX Plotter");

    mainWindow.setCentralWidget(new ContentView());

    mainWindow.show();

    return mainApp.exec();
}
