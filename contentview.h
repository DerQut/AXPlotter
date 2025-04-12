#ifndef CONTENTVIEW_H
#define CONTENTVIEW_H

#pragma once

#include <QWidget>
#include <QString>
#include <QList>

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
};

#endif // CONTENTVIEW_H
