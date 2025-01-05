#ifndef SIDEBARVIEW_H
#define SIDEBARVIEW_H

#pragma once

#include <QWidget>

#include "themefile.h"

class SideBarView : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarView(QWidget *parent = 0);

private:
    QWidget* background;

signals:

public slots:
    void setDefaultTheme(int shouldSet);
    void setDarkTheme(int shouldSet);
    void setTheme(Theme theme);

};

class SideBarViewEntry : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarViewEntry(SideBarView *parent = 0);

private:
    bool isExpanded;

signals:

public slots:
    void toggleExpanded();

};

#endif // SIDEBARVIEW_H
