#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "sidebarview.h"

SideBarEntryView::SideBarEntryView(QWidget *expandableWidget, SideBarView *parent) :
    QWidget(parent)
{
    this->sideBarView = parent;
    this->expandableWidget = expandableWidget;
    this->isExpanded = true;

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setSpacing(0);

    QPushButton* button = new QPushButton(this);
    button->setContentsMargins(0, 0, 0, 0);
    mainVStack->addWidget(button);

    mainVStack->addWidget(this->expandableWidget);

    this->setLayout(mainVStack);

    connect(button, SIGNAL(clicked()), this, SLOT(toggleExpanded()));
}


void SideBarEntryView::toggleExpanded() {
    this->isExpanded = !isExpanded;
    if (isExpanded) {
        this->expand();
        return;
    }

    this->retract();
}


void SideBarEntryView::expand() {
    this->expandableWidget->show();
}


void SideBarEntryView::retract() {
    this->expandableWidget->hide();
}
