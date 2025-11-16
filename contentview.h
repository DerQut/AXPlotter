#ifndef CONTENTVIEW_H
#define CONTENTVIEW_H

#pragma once

#include <QWidget>
#include <QString>
#include <QList>

#include "axdataseries.h"

class SideBarView;
class DetailView;
class AXInterpreter;

class ContentView : public QWidget
{
    Q_OBJECT
public:
    explicit ContentView(QWidget *parent = 0);
    SideBarView* sideBarView;
    DetailView* detailView;
    AXInterpreter* axinterpreter;

    QString scriptFile;

    QList<QString> recentFiles;
    QList<AXDataSeries> inferredVariables;

signals:
    void recentFilesChanged();

public slots:
    void setTheme(int idClicked);
    void obtainScriptFile();
    void readScriptFile(QString newScriptFile);
    void saveScriptFile();
    void saveScriptFileAs();
    void shiftRecentFiles();
    void askToCompile();
    void refreshInferredVariables();
    void askToOpenCSVDir();
    void openCSVDir(QString dirName);
};

#endif // CONTENTVIEW_H
