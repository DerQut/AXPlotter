#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

#include "detailview.h"
#include "contentview.h"
#include "editorview.h"

DetailView::DetailView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;

    // Creating a QTabWidget instance
    QTabWidget* tabView = new QTabWidget(this);
    tabView->setTabPosition(QTabWidget::North);

    // Creating tabs for the QTabWidget instance
    editorView = new EditorView(this);

    QWidget* tempRect2 = new QWidget(this);
    tempRect2->setStyleSheet("background-color: blue;");

    // Adding the temp widgets to the QTabWidget instance
    tabView->addTab(editorView, "Edit");
    tabView->addTab(tempRect2, "Blue");

    // Creating a dummy layout to host the QTabWidget instance
    QVBoxLayout* dummyLayout = new QVBoxLayout(this);
    dummyLayout->addWidget(tabView);

    // Removing the default padding introduced by the dummy layout
    dummyLayout->setMargin(0);

    this->setLayout(dummyLayout);

}
