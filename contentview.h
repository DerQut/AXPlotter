#ifndef CONTENTVIEW_H
#define CONTENTVIEW_H

#pragma once

#include <QWidget>

class SideBarView;
class DetailView;

class ContentView : public QWidget
{
    Q_OBJECT
public:
    explicit ContentView(QWidget *parent = 0);
    SideBarView* sideBarView;
    DetailView* detailView;

signals:

public slots:
    void setTheme(int idClicked);
};

#endif // CONTENTVIEW_H
