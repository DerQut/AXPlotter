#include <QWidget>
#include <QVBoxLayout>

#include "sidebarview.h"

SideBarViewEntry::SideBarViewEntry(SideBarView *parent) :
    QWidget(parent)
{
    isExpanded = true;

    QVBoxLayout* mainVStack = new QVBoxLayout();

    this->setLayout(mainVStack);
}

void SideBarViewEntry::toggleExpanded() {
    this->isExpanded = !isExpanded;
}
