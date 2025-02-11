#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QGradient>
#include <QApplication>

#include "sidebarview.h"
#include "contentview.h"
#include "themefile.h"

SideBarView::SideBarView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;
    this->setContentsMargins(2, 0, 2, 2);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    // Creating the widget to host the background gradient
    background = new QWidget();

    QVBoxLayout* mainVStack = new QVBoxLayout();

    mainVStack->addStretch();

    QWidget* dummyZStackWidget = new QWidget();
    dummyZStackWidget->setLayout(mainVStack);

    mainZStack->addWidget(background);
    mainZStack->addWidget(dummyZStackWidget);

    this->setLayout(mainZStack);
}

void SideBarView::setDarkTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #454545, stop:1 #808080);");
    saveTheme("theme.cfg", DARK);
}

void SideBarView::setDefaultTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #6375d7, stop:1 #7ba2e6);");
    saveTheme("theme.cfg", DEFAULT);
}

void SideBarView::setClassicTheme()
{
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #d2d0c9, stop:1 #d2d0c9);");
    saveTheme("theme.cfg", CLASSIC);
}

void SideBarView::setTheme(Theme theme) {
    switch (theme) {
    case CLASSIC:
        this->setClassicTheme();
        break;
    case DARK:
        this->setDarkTheme();
        break;
    default:
        this->setDefaultTheme();
    }
}
