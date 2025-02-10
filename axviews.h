#ifndef AXVIEWS_H
#define AXVIEWS_H

#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QCheckBox>

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


class ContentView : public QWidget
{
    Q_OBJECT
public:
    explicit ContentView(QWidget *parent = 0);
    SideBarView* sideBarView;

signals:

public slots:

};


class AXMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit AXMenuBar(ContentView* parent = 0);
    ContentView* contentView;

private:

signals:

public slots:

};


class AXMBViewMenu : public QMenu
{
    Q_OBJECT
public:
    explicit AXMBViewMenu(const QString &title, AXMenuBar* parent = 0);
    AXMenuBar* axMenuBar;

private:
    QCheckBox* defaultThemeCheck;
    QCheckBox* darkThemeCheck;

signals:

public slots:

};


class AXToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit AXToolBar(ContentView *parent = 0);
    ContentView* contentView;

signals:

public slots:

};



#endif // AXVIEWS_H
