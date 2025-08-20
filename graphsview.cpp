#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

#include "graphsview.h"
#include "contentview.h"

GraphsView::GraphsView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setContentsMargins(0, 0, 0, 0);
    mainVStack->setSpacing(0);

    scrollWidget = new QWidget(this);
    scrollWidget->setMinimumHeight(2137);
    scrollArea->setWidget(scrollWidget);

    mainVStack->addWidget(scrollArea);
}


void GraphsView::updatePlots(QString directoryName) {
    return;
}

void GraphsView::setPlotsXRange(int xMin, int yMin) {
    return;
}
