#include "axmenubar.h"
#include "contentview.h"
#include "sidebarview.h"

#include <QWidget>
#include <QAction>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QCheckBox>

AXMenuBar::AXMenuBar(ContentView* parent) :
    QMenuBar(parent)
{
    contentView = parent;

    QMenu* fileMenu = new QMenu("&File", this);
    this->addMenu(fileMenu);

    QMenu* editMenu = new QMenu("&Edit", this);
    this->addMenu(editMenu);

    AXMBViewMenu* viewMenu = new AXMBViewMenu("&View", this);
    this->addMenu(viewMenu);

    QMenu* helpMenu = new QMenu("&Help", this);
    this->addMenu(helpMenu);
}


AXMBViewMenu::AXMBViewMenu(const QString &title, AXMenuBar* parent) :
    QMenu(title, parent)
{
    axMenuBar = parent;

    QButtonGroup* themeGroup = new QButtonGroup(this);
    themeGroup->setExclusive(1);

    lunaCheck = new QCheckBox("&Luna", this);
    zuneCheck = new QCheckBox("&Zune", this);

    themeGroup->addButton(lunaCheck, 0);
    themeGroup->addButton(zuneCheck, 1);
    lunaCheck->setChecked(1);

    QVBoxLayout* themeVStack = new QVBoxLayout(this);
    themeVStack->setSpacing(0);
    themeVStack->addWidget(lunaCheck);
    themeVStack->addWidget(zuneCheck);

    QWidget* themeDummyWidget = new QWidget(this);
    themeDummyWidget->setLayout(themeVStack);
    themeDummyWidget->setContentsMargins(0, 0, 0, 0);

    QMenu* themeSubMenu = this->addMenu("&Theme");

    QWidgetAction* selectTheme = new QWidgetAction(this);
    selectTheme->setDefaultWidget(themeDummyWidget);
    themeSubMenu->addAction(selectTheme);

    connect(lunaCheck, SIGNAL(stateChanged(int)), this->axMenuBar->contentView->sideBarView, SLOT(setLunaTheme(int)));
    connect(zuneCheck, SIGNAL(stateChanged(int)), this->axMenuBar->contentView->sideBarView, SLOT(setZuneTheme(int)));
}

