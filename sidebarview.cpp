#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QGradient>
#include <QApplication>

#include "sidebarview.h"
#include "contentview.h"

SideBarView::SideBarView(ContentView *parent) :
    QWidget(parent)
{
    this->setContentsMargins(1, 1, 1, 1);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    QWidget* background = new QWidget();
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
