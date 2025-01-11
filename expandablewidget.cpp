#include <QWidget>

#include "sidebarview.h"

ExpandableWidget::ExpandableWidget(SideBarEntryView *parent) :
    QWidget(parent)
{
    sideBarEntryView = parent;
    defaultHeight = 0;
}

