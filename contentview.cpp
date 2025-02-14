#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>

#include "contentview.h"
#include "sidebarview.h"
#include "detailview.h"

#include "themefile.h"


ContentView::ContentView(QWidget* parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainHStack = new QHBoxLayout(this);
    mainHStack->setMargin(0);
    mainHStack->setSpacing(0);

    sideBarView = new SideBarView(this);
    sideBarView->setFixedWidth(210);
    sideBarView->setTheme(getTheme("theme.cfg"));

    detailView = new DetailView(this);
    detailView->setContentsMargins(0, 0, 0, 0);

    mainHStack->addWidget(sideBarView);
    mainHStack->addWidget(detailView);

    this->setLayout(mainHStack);
}


void ContentView::setTheme(int idClicked) {
    this->sideBarView->setTheme((Theme) idClicked);
}


void ContentView::readScriptFile() {
    this->scriptFile = QFileDialog::getOpenFileName(this, tr("Select script file"));
    qDebug() << this->scriptFile;

    if (scriptFile == "") { return; }

    this->sideBarView->setFileLabel(scriptFile);
}
