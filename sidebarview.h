#ifndef SIDEBARVIEW_H
#define SIDEBARVIEW_H

#pragma once

#include <QWidget>

#include "themefile.h"


class ContentView;


class SideBarView : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarView(ContentView *parent = 0);
    void setDefaultTheme();
    void setClassicTheme();
    void setDarkTheme();

private:
    ContentView* contentView;
    QWidget* background;

signals:

public slots:
    void setTheme(Theme theme);

};

#endif // SIDEBARVIEW_H
