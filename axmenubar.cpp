#include "axmenubar.h"
#include "contentview.h"

#include <QWidget>
#include <QAction>
#include <QString>
#include <QWidgetAction>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

AXMenuBar::AXMenuBar(ContentView* parent) :
    QMenuBar(parent)
{
    contentView = parent;
    themePicker = new QComboBox(this);

    themePicker->addItem("Luna");
    themePicker->addItem("Zune");

    QMenu* viewMenu = this->addMenu("&View");
    //viewMenu->addAction("Theme", contentView->sideBarView, SLOT(setBackgroundColors()));

    QHBoxLayout* themePickerHStack = new QHBoxLayout(this);
    themePickerHStack->addWidget(new QLabel("Theme:"));
    themePickerHStack->addWidget(themePicker);

    QWidget* themePickerDummyWidget = new QWidget();
    themePickerDummyWidget->setLayout(themePickerHStack);

    QWidgetAction* selectTheme = new QWidgetAction(this);

    selectTheme->setDefaultWidget(themePickerDummyWidget);

    viewMenu->addAction(selectTheme);
}

void AXMenuBar::showFileMenu()
{
    printf("Read\r\n");
}

void AXMenuBar::showThemeMenu()
{
    printf("Theme\r\n");
}
