#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPushButton>
#include <QDebug>

#include "inferredvariablesview.h"
#include "contentview.h"
#include "toggleviewhost.h"

InferredVariablesView::InferredVariablesView(ContentView* parent) :
    ToggleViewHost(parent)
{
    this->toggleButton->setText("Inferred variables");

}

void InferredVariablesView::refreshVariables() {

}
