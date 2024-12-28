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
    this->setContentsMargins(0, 0, 0, 0);

    QStackedLayout* mainZStack = new QStackedLayout();
    mainZStack->setStackingMode(QStackedLayout::StackAll);

    QWidget* background = new QWidget();
    background->setStyleSheet("background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #888888, stop:1 #454545);");

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
