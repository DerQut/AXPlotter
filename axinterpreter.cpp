#include <QDebug>
#include <QLabel>

#include "axinterpreter.h"
#include "contentview.h"

AXInterpreter::AXInterpreter(ContentView* parent) :
    QMainWindow(parent)
{
    this->contentView = parent;

    QLabel* placeholder = new QLabel("Test");
    this->setCentralWidget(placeholder);
    this->hide();
}



void AXInterpreter::startCompilation() {
    this->show();
}
