#include <QWidget>
#include <QToolBar>

#include "axtoolbar.h"
#include "contentview.h"

AXToolBar::AXToolBar(ContentView *parent) :
    QToolBar(parent)
{
    this->contentView = parent;
}
