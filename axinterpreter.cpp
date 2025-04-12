#include <QDebug>

#include "axinterpreter.h"
#include "contentview.h"

AXInterpreter::AXInterpreter(ContentView* parent) :
    QMessageBox(parent)
{
    this->contentView = parent;

    this->setText("Test");
}



void AXInterpreter::startCompilation() {
    qDebug() << "Test";
    this->exec();
}
