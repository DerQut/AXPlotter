#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "contentview.h"
#include "sidebarview.h"

ContentView::ContentView(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainHStack = new QHBoxLayout(this);
    mainHStack->setMargin(0);
    mainHStack->setSpacing(0);

    SideBarView *sideBarView = new SideBarView(this);
    sideBarView->setFixedWidth(200);

    QWidget *rightRect = new QWidget(this);
    rightRect->setStyleSheet("background-color: blue;");
    rightRect->setContentsMargins(0, 0, 0, 0);
    rightRect->setFixedWidth(400);

    mainHStack->addWidget(sideBarView);
    mainHStack->addWidget(rightRect);

    this->setLayout(mainHStack);
}
