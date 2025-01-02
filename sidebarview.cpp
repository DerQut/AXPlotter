#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QGradient>
#include <QApplication>

#include "sidebarview.h"

SideBarView::SideBarView(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(2, 0, 0, 2);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    // Creating the background gradient, stylized after the Luna theme built into Windows XP
    background = new QWidget();
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #6375d7, stop:1 #7ba2e6);");

    QVBoxLayout* mainVStack = new QVBoxLayout();

    QLabel* titleLabel = new QLabel("AX Plotter");
    mainVStack->addWidget(titleLabel);
    mainVStack->addStretch();

    QWidget* dummyZStackWidget = new QWidget();
    dummyZStackWidget->setLayout(mainVStack);

    mainZStack->addWidget(background);
    mainZStack->addWidget(dummyZStackWidget);

    this->setLayout(mainZStack);
}

void SideBarView::setDarkTheme(int shouldSet)
{
    if (!shouldSet) {return;};
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #454545, stop:1 #808080);");
}

void SideBarView::setDefaultTheme(int shouldSet)
{
    if (!shouldSet) {return;};
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:1, x2:0 y2:0, stop:0 #6375d7, stop:1 #7ba2e6);");
}
