#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "axviews.h"

#include "themefile.h"


ContentView::ContentView(QWidget* parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainHStack = new QHBoxLayout(this);
    mainHStack->setMargin(0);
    mainHStack->setSpacing(0);

    sideBarView = new SideBarView(this);
    sideBarView->setFixedWidth(210);
    sideBarView->setTheme(getTheme("theme.cfg"));

    QWidget *rightRect = new QWidget(this);
    rightRect->setStyleSheet("background-color: white;");
    rightRect->setContentsMargins(0, 0, 0, 0);
    //rightRect->setFixedWidth(400);

    mainHStack->addWidget(sideBarView);
    mainHStack->addWidget(rightRect);

    this->setLayout(mainHStack);
}
