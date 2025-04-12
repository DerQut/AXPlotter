#include <fstream>

#include <QWidget>
#include <QSplitter>
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
    QHBoxLayout* dummyLayout = new QHBoxLayout();

    recentFiles << "" << "" << "" << "" << "";

    QSplitter* mainSplitter = new QSplitter(this);
    mainSplitter->setContentsMargins(0, 0, 0, 0);
    mainSplitter->setHandleWidth(5);
    mainSplitter->setChildrenCollapsible(false);

    sideBarView = new SideBarView(this);
    //sideBarView->setFixedWidth(210);
    sideBarView->setTheme(getTheme("theme.cfg"));

    detailView = new DetailView(this);
    detailView->setContentsMargins(0, 0, 0, 0);

    mainSplitter->addWidget(sideBarView);
    mainSplitter->addWidget(detailView);

    mainSplitter->setSizes(QList<int>() << 150 << 480-150);

    dummyLayout->addWidget(mainSplitter);
    this->setLayout(dummyLayout);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(obtainScriptFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(saveScriptFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S + Qt::SHIFT), this, SLOT(saveScriptFileAs()));
}


void ContentView::setTheme(int idClicked) {
    this->sideBarView->setTheme((Theme) idClicked);
}


void ContentView::obtainScriptFile() {
    QString newScriptFile = QFileDialog::getOpenFileName(this, tr("Select script file"));
    qDebug() << this->scriptFile;

    if (newScriptFile == "") { return; }

    QFileInfo fileInfo(newScriptFile);
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

    this->scriptFile = newScriptFile;
    this->sideBarView->setFileLabel(scriptFile);
    this->detailView->editorView->readRecipeFile(scriptFile);

    this->shiftRecentFiles();
}


void ContentView::saveScriptFile() {

    QFileInfo fileInfo(scriptFile);
    QMessageBox messageBox;

    // Empty file name fallback
    if (scriptFile == "") { saveScriptFileAs(); }

    // Missing file extention fallback
    if (scriptFile.count(".") != 1) {
        scriptFile.append(".txt");
    }

    // Additional fallback for a semi-empty file name
    if (scriptFile.at(0) == '.') {
        messageBox.setText("File name \"" + scriptFile + "\" is invalid. Unable to save file.");
        messageBox.exec();
        return;
    }

    // Opening the given file
    std::ofstream file;
    file.open(scriptFile.toStdString());

    // Error handling
    if (!file.is_open()) {
        qDebug() << "Error writing to file";
        return;
    }

    // Saving Theme to file
    file << (this->detailView->editorView->textEdit->document()->toPlainText().toStdString());
    file.close();

    qDebug() << "File " << scriptFile << " saved!";

}


void ContentView::saveScriptFileAs() {
    this->scriptFile = QFileDialog::getSaveFileName(this, tr("Select script file"));
    qDebug() << this->scriptFile;

    QFileInfo fileInfo(scriptFile);
    QMessageBox messageBox;

    const QString fileName = fileInfo.fileName();
    qDebug() << fileName;

    if (fileName == "") {
        messageBox.setText("File name is empty. Unable to save file.");
        messageBox.exec();
        return;
    }

    if (fileName.count('.') > 1) {
        messageBox.setText("File name \"" + fileName + "\" contains more than one '.' character. Unable to save file.");
        messageBox.exec();
        return;
    }

    if (fileName.count(' ') > 0) {
        messageBox.setText("File name \"" + fileName + "\" contains a '  ' character. Unable to save file.");
        messageBox.exec();
        return;
    }

    this->saveScriptFile();

    this->sideBarView->setFileLabel(scriptFile);
    this->detailView->editorView->readRecipeFile(scriptFile);
}


void ContentView::shiftRecentFiles() {
    recentFiles.removeLast();
    recentFiles.prepend(scriptFile);

    emit this->recentFilesChanged();
}


void ContentView::readScriptFile(QString newScriptFile) {
    qDebug() << this->scriptFile;

    if (newScriptFile == "") { return; }

    QFileInfo fileInfo(newScriptFile);
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

    this->scriptFile = newScriptFile;
    this->sideBarView->setFileLabel(scriptFile);
    this->detailView->editorView->readRecipeFile(scriptFile);

}
