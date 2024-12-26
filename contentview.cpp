#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "contentview.h"

ContentView::ContentView(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainHStack = new QHBoxLayout(this);
    mainHStack->setMargin(0);
    mainHStack->setSpacing(0);

    QWidget *leftRect = new QWidget(this);
    leftRect->setStyleSheet("background-color: red;");
    leftRect->setContentsMargins(0, 0, 0, 0);

    QWidget *rightRect = new QWidget(this);
    rightRect->setStyleSheet("background-color: blue;");
    rightRect->setContentsMargins(0, 0, 0, 0);

    mainHStack->addWidget(leftRect);
    mainHStack->addWidget(rightRect);

    this->setLayout(mainHStack);
}
