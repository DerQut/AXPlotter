#include "inferredvariablesview.h"
#include "contentview.h"

InferredVariablesView::InferredVariablesView(ContentView* parent) :
    QWidget(parent)
{
    this->contentView = parent;
}
