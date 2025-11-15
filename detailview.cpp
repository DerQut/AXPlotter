#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

#include "detailview.h"
#include "contentview.h"
#include "editorview.h"
#include "graphsview.h"
#include "stackedgraphsview.h"

DetailView::DetailView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;
    this->setContentsMargins(0, 0, 0, 0);

    // Creating a QTabWidget instance
    tabView = new QTabWidget(this);
    tabView->setTabPosition(QTabWidget::North);

    // Creating the editor tab
    editorView = new EditorView(this);

    // Creating the graphs tab
    graphsView = new GraphsView(this->contentView);

    // ...
    stackedGraphsView = new StackedGraphsView(this->contentView);


    // Adding the temp widgets to the QTabWidget instance
    tabView->addTab(editorView, "Edit");
    tabView->addTab(graphsView, "Graphs");
    tabView->addTab(stackedGraphsView, "Stacked graph");

    // Creating a dummy layout to host the QTabWidget instance
    QVBoxLayout* dummyLayout = new QVBoxLayout(this);
    dummyLayout->addWidget(tabView);

    // Removing the default padding introduced by the dummy layout
    dummyLayout->setMargin(0);

    this->setLayout(dummyLayout);

}
