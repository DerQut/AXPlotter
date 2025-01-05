#include "axmenubar.h"
#include "contentview.h"
#include "sidebarview.h"
#include "themefile.h"

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

    defaultThemeCheck = new QCheckBox("&Default", this);
    darkThemeCheck = new QCheckBox("&Dark", this);

    themeGroup->addButton(defaultThemeCheck, 0);
    themeGroup->addButton(darkThemeCheck, 1);

    defaultThemeCheck->setChecked(1);
    darkThemeCheck->setChecked(getTheme("theme.cfg") == DARK);

    QVBoxLayout* themeVStack = new QVBoxLayout(this);
    themeVStack->setSpacing(0);
    themeVStack->addWidget(defaultThemeCheck);
    themeVStack->addWidget(darkThemeCheck);

    QWidget* themeDummyWidget = new QWidget(this);
    themeDummyWidget->setLayout(themeVStack);
    themeDummyWidget->setContentsMargins(0, 0, 0, 0);

    QMenu* themeSubMenu = this->addMenu("&Theme");

    QWidgetAction* selectTheme = new QWidgetAction(this);
    selectTheme->setDefaultWidget(themeDummyWidget);
    themeSubMenu->addAction(selectTheme);

    connect(defaultThemeCheck, SIGNAL(stateChanged(int)), this->axMenuBar->contentView->sideBarView, SLOT(setDefaultTheme(int)));
    connect(darkThemeCheck, SIGNAL(stateChanged(int)), this->axMenuBar->contentView->sideBarView, SLOT(setDarkTheme(int)));
}

