#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>

#include "sidebarview.h"

FileEntryView::FileEntryView(SideBarEntryView *parent) :
    QWidget(parent)
{
    this->sideBarEntryView = parent;

    QStackedLayout* mainZStack = new QStackedLayout(this);
    QWidget* background = new QWidget(this);
    background->setStyleSheet("background-color: white");
    background->setContentsMargins(0, 0, 0, 0);
    mainZStack->addWidget(background);

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setSpacing(0);

    QWidget* dummyWidget = new QWidget(this);
    dummyWidget->setLayout(mainVStack);
    dummyWidget->setContentsMargins(0, 0, 0, 0);

    mainZStack->addWidget(dummyWidget);







    this->setLayout(mainZStack);
    this->setContentsMargins(0, 0, 0, 0);
}
