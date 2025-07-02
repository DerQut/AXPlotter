#include <QWidget>
#include <QAction>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>

#include "axmenubar.h"
#include "contentview.h"
#include "sidebarview.h"
#include "themefile.h"
#include "axinterpreter.h"

AXMenuBar::AXMenuBar(ContentView* parent) :
    QMenuBar(parent)
{
    contentView = parent;

    // Creating QMenu instances
    QMenu* fileMenu = new AXMBFileMenu("&File", this);
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
    defaultThemeButton = new QRadioButton("Default", this);
    darkThemeButton = new QRadioButton("Dark", this);
    classicThemeButton = new QRadioButton("Classic", this);

    /*
     * Adding the created QRadioButton instances to the QButtonGroup instance
     * using the Theme enum as the button ID
    */
    themeGroup->addButton(defaultThemeButton, DEFAULT);
    themeGroup->addButton(darkThemeButton, DARK);
    themeGroup->addButton(classicThemeButton, CLASSIC);

    // Checking the appropriate check box at startup
    defaultThemeButton->setChecked(1);
    darkThemeButton->setChecked(getTheme("theme.cfg") == DARK);
    classicThemeButton->setChecked(getTheme("theme.cfg") == CLASSIC);

    // Creating a QVBoxLayout instance to host the buttons inside the theme sub-menu
    QVBoxLayout* themeVStack = new QVBoxLayout(this);
    themeVStack->setSpacing(0);
    themeVStack->addWidget(defaultThemeButton);
    themeVStack->addWidget(darkThemeButton);
    themeVStack->addWidget(classicThemeButton);

    QWidget* themeDummyWidget = new QWidget(this);
    themeDummyWidget->setLayout(themeVStack);
    themeDummyWidget->setContentsMargins(0, 0, 0, 0);

    QMenu* themeSubMenu = this->addMenu("&Theme");

    QWidgetAction* selectTheme = new QWidgetAction(this);
    selectTheme->setDefaultWidget(themeDummyWidget);
    themeSubMenu->addAction(selectTheme);

    connect(themeGroup, SIGNAL(buttonClicked(int)), this->axMenuBar->contentView, SLOT(setTheme(int)));
}


AXMBFileMenu::AXMBFileMenu(const QString &title, AXMenuBar* parent) :
    QMenu(title, parent)
{
    axMenuBar = parent;

    // Creating QActions for file operations
    QAction* fileOpenAction = this->addAction("Open recipe file");
    QAction* fileSaveAction = this->addAction("Save recipe file");
    QAction* fileSaveAsAction = this->addAction("Save recipe file as");

    QAction* fileStartCompilation = this->addAction("Start compilation");

    // Connecting all the necessary signals
    connect(fileOpenAction, SIGNAL(triggered()), this->axMenuBar->contentView, SLOT(obtainScriptFile()));
    connect(fileSaveAction, SIGNAL(triggered()), this->axMenuBar->contentView, SLOT(saveScriptFile()));
    connect(fileSaveAsAction, SIGNAL(triggered()), this->axMenuBar->contentView, SLOT(saveScriptFileAs()));

    connect(fileStartCompilation, SIGNAL(triggered()), this->axMenuBar->contentView->axinterpreter, SLOT(startCompilation()));
}

