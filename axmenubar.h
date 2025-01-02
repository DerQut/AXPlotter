#ifndef AXMENUBAR_H
#define AXMENUBAR_H

#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <QButtonGroup>
#include <QCheckBox>

#include "contentview.h"

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

#endif // AXMENUBAR_H
