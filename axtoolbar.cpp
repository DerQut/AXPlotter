#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QStyle>
#include <QLabel>
#include <QPushButton>
#include <QSize>

#include "axviews.h"

AXToolBar::AXToolBar(ContentView *parent) :
    QToolBar(parent)
{
    this->contentView = parent;

    QPushButton* openButton = new QPushButton(this->style()->standardIcon(QStyle::SP_DirOpenIcon), "&Open", this);
    openButton->setFlat(true);
    openButton->setStyleSheet("");

    this->addWidget(openButton);

    this->setMovable(false);

}
