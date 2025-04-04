#include "mainwindow.h"
#include "contentview.h"
#include "axtoolbar.h"
#include "axmenubar.h"

#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QMenuBar>
#include <QDebug>


// Created by Marcel Cholodecki on 2024.12.24

int main(int argc, char *argv[])
{
    QApplication mainApp(argc, argv);
    MainWindow* mainWindow = new MainWindow();
    mainWindow->setWindowTitle("AX Plotter");

    ContentView* contentView = new ContentView();
    mainWindow->setCentralWidget(contentView);

    AXMenuBar* menubar = new AXMenuBar(contentView);
    mainWindow->setMenuBar(menubar);

    AXToolBar* toolbar = new AXToolBar(contentView);
    mainWindow->addToolBar(toolbar);

    // Disabling the visibility of the toolbar until the general UI is figured out
    toolbar->setVisible(false);

    mainWindow->show();
    mainWindow->resize(640, 480);

    return mainApp.exec();
}
