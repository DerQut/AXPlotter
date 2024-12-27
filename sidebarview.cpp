#include <QWidget>
#include <QVBoxLayout>

#include "sidebarview.h"
#include "contentview.h"

SideBarView::SideBarView(ContentView *parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setSpacing(0);
    mainVStack->setMargin(0);

    QWidget* placeholderRect = new QWidget();
    placeholderRect->setStyleSheet("background-color: red;");
    mainVStack->addWidget(placeholderRect);


    this->setLayout(mainVStack);
}
