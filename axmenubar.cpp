#include <QWidget>
#include <QAction>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QCheckBox>

#include "axmenubar.h"
#include "contentview.h"
#include "sidebarview.h"
#include "themefile.h"

AXMenuBar::AXMenuBar(ContentView* parent) :
    QMenuBar(parent)
{
    contentView = parent;

    // Creating QMenu instances
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

    // Creating an exclusive QButtonGroup fot the Theme picker
    QButtonGroup* themeGroup = new QButtonGroup(this);
    themeGroup->setExclusive(1);

    // Creating QCheckBox instances for the Theme picker
    defaultThemeCheck = new QCheckBox("Default", this);
    darkThemeCheck = new QCheckBox("Dark", this);
    classicThemeCheck = new QCheckBox("Classic", this);

    /*
     * Adding the created QCheckBox instances to the QButtonGroup instance
     * using the Theme enum as the button ID
    */
    themeGroup->addButton(defaultThemeCheck, DEFAULT);
    themeGroup->addButton(darkThemeCheck, DARK);
    themeGroup->addButton(classicThemeCheck, CLASSIC);

    // Checking the appropriate check box at startup
    defaultThemeCheck->setChecked(1);
    darkThemeCheck->setChecked(getTheme("theme.cfg") == DARK);
    classicThemeCheck->setChecked(getTheme("theme.cfg") == CLASSIC);

    // Creating a QVBoxLayout instance to host the buttons inside the theme sub-menu
    QVBoxLayout* themeVStack = new QVBoxLayout(this);
    themeVStack->setSpacing(0);
    themeVStack->addWidget(defaultThemeCheck);
    themeVStack->addWidget(darkThemeCheck);
    themeVStack->addWidget(classicThemeCheck);

    QWidget* themeDummyWidget = new QWidget(this);
    themeDummyWidget->setLayout(themeVStack);
    themeDummyWidget->setContentsMargins(0, 0, 0, 0);

    QMenu* themeSubMenu = this->addMenu("&Theme");

    QWidgetAction* selectTheme = new QWidgetAction(this);
    selectTheme->setDefaultWidget(themeDummyWidget);
    themeSubMenu->addAction(selectTheme);

    connect(themeGroup, SIGNAL(buttonClicked(int)), this->axMenuBar->contentView, SLOT(setTheme(int)));
}

