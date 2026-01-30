#ifndef AXMENUBAR_H
#define AXMENUBAR_H

#pragma once

#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <QButtonGroup>
#include <QRadioButton>

class ContentView;
class XMLConfigWindow;
class HelpWindow;


class AXMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit AXMenuBar(ContentView* parent = 0);
    ContentView* contentView;
    XMLConfigWindow* xmlConfigWindow;
    HelpWindow* helpWindow;

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
    QRadioButton* defaultThemeButton;
    QRadioButton* darkThemeButton;
    QRadioButton* classicThemeButton;

signals:

public slots:

};


class AXMBFileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit AXMBFileMenu(const QString &title, AXMenuBar* parent = 0);
    AXMenuBar* axMenuBar;

private:

signals:

public slots:

};

#endif // AXMENUBAR_H
