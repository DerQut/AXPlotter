#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>

#include "contentview.h"
#include "sidebarview.h"
#include "detailview.h"
#include "editorview.h"

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

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(obtainScriptFile()));
}


void ContentView::setTheme(int idClicked) {
    this->sideBarView->setTheme((Theme) idClicked);
}


void ContentView::obtainScriptFile() {
    this->scriptFile = QFileDialog::getOpenFileName(this, tr("Select script file"));
    qDebug() << this->scriptFile;

    if (scriptFile == "") { return; }

    QFileInfo fileInfo(scriptFile);
    QMessageBox messageBox;

    const QString fileName = fileInfo.fileName();
    qDebug() << fileName;

    if (fileName.count('.') > 1) {
        messageBox.setText("File name \"" + fileName + "\" contains more than one '.' character. Unable to read file.");
        messageBox.exec();
        return;
    }

    if (fileName.count(' ') > 0) {
        messageBox.setText("File name \"" + fileName + "\" contains a ' ' character. Unable to read file.");
        messageBox.exec();
        return;
    }

    this->sideBarView->setFileLabel(scriptFile);
    this->detailView->editorView->readRecipeFile(scriptFile);
}
