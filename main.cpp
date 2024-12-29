#include "mainwindow.h"
#include "contentview.h"
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

    QPalette::ColorGroup group = QPalette::Active;
    QPalette::ColorRole role = QPalette::Base;

    QPalette palette = QApplication::palette();
    qDebug() << "palette before:" << palette.color(group, role).name();

    AXMenuBar* menubar = new AXMenuBar(mainWindow);

    mainWindow->setMenuBar(menubar);

    mainWindow->setCentralWidget(new ContentView());

    mainWindow->show();
    printf("Window size: %d x %d\n", mainWindow->width(), mainWindow->height());

    return mainApp.exec();
}
