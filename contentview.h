#ifndef CONTENTVIEW_H
#define CONTENTVIEW_H

#pragma once

#include <QWidget>
#include <QString>

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

signals:

public slots:
    void setTheme(int idClicked);
    void readScriptFile();
};

#endif // CONTENTVIEW_H
