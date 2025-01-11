#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "sidebarview.h"

FileExpandableWidget::FileExpandableWidget(SideBarEntryView *parent) :
    ExpandableWidget(parent)
{
    QStackedLayout* mainZStack = new QStackedLayout(this);

    QWidget* background = new QWidget(this);
    background->setStyleSheet("background-color: white");
    background->setContentsMargins(0, 0, 0, 0);

    mainZStack->addWidget(background);


    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setSpacing(0);

    QWidget* dummyWidget = new QWidget(this);
    dummyWidget->setLayout(mainVStack);
    dummyWidget->setContentsMargins(0, 0, 0, 0);

    mainZStack->addWidget(dummyWidget);


    this->setLayout(mainZStack);
    this->setContentsMargins(0, 0, 0, 0);

}
