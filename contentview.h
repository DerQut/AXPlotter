#ifndef CONTENTVIEW_H
#define CONTENTVIEW_H

#pragma once

#include <QWidget>
#include <QString>
#include <QList>

class SideBarView;
class DetailView;

class ContentView : public QWidget
{
    Q_OBJECT
public:
    explicit ContentView(QWidget *parent = 0);
    SideBarView* sideBarView;
    DetailView* detailView;

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
};

#endif // CONTENTVIEW_H
