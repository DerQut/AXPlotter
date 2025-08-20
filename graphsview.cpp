#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDir>
#include <QString>
#include <QStringList>

#include "graphsview.h"
#include "contentview.h"
#include "axinterpreter.h"

GraphsView::GraphsView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setContentsMargins(0, 0, 0, 0);
    mainVStack->setSpacing(0);

    QWidget* scrollWidget = new QWidget(this);
    scrollArea->setWidget(scrollWidget);

    this->scrollVStack = new QVBoxLayout(scrollWidget);

    QPushButton* forceUpdateButton = new QPushButton("Update", this);
    connect(forceUpdateButton, &QPushButton::clicked, this, [this]() {
        this->updatePlots(this->contentView->axinterpreter->baseFolder.absolutePath());
    });
    mainVStack->addWidget(forceUpdateButton);

    mainVStack->addWidget(scrollArea);
}


void GraphsView::updatePlots(QString directoryName) {

    QLayoutItem* itemToRemove;
    while ((itemToRemove = this->scrollVStack->takeAt(0)) != nullptr) {
        if (itemToRemove->widget()) {
            delete itemToRemove->widget();
        }
        delete itemToRemove;
    }


    QDir csvDir (directoryName);
    QStringList csvFiles = csvDir.entryList(QStringList() << "*.csv", QDir::Files);

    // Add the graphs (tbd)

    QLabel* dirLabel = new QLabel(directoryName, this);

    this->scrollVStack->addWidget(dirLabel);

    this->scrollVStack->addStretch();
}

void GraphsView::setPlotsXRange(int xMin, int yMin) {
    return;
}
