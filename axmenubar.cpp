#include "axmenubar.h"

#include <QAction>
#include <QString>

AXMenuBar::AXMenuBar(QWidget *parent) :
    QMenuBar(parent)
{
        QMenu* fileMenu = this->addMenu("&File");
        fileMenu->addAction("Read", this, SLOT(showFileMenu()));
}

void AXMenuBar::showFileMenu()
{
    printf("Read\r\n");
}
