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


class SideBarEntryView : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarEntryView(QWidget* expandableWidget, SideBarView* parent = 0);

private:
    SideBarView* sideBarView;
    bool isExpanded;
    QWidget* expandableWidget;

signals:

public slots:
    void toggleExpanded();
    void expand();
    void retract();

};


class FileEntryView : public QWidget
{
    Q_OBJECT
public:
    explicit FileEntryView(SideBarEntryView* parent = 0);
private:
    SideBarEntryView* sideBarEntryView;

signals:

public slots:

};

#endif // SIDEBARVIEW_H
