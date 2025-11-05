#include <QVBoxLayout>

#include "stackedgraphsview.h"
#include "contentview.h"

StackedGraphsView::StackedGraphsView(ContentView* parent) :
    QWidget(parent)
{
    this->contentView = parent;
    QVBoxLayout* mainVStack = new QVBoxLayout();

    this->setLayout(mainVStack);
}
