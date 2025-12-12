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
#include "axinterpreter.h"
#include "axdataseries.h"
#include "inferredvariablesview.h"
#include "stackedgraphsview.h"

#include "themefile.h"


ContentView::ContentView(QWidget* parent) :
    QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* dummyLayout = new QHBoxLayout();
    dummyLayout->setContentsMargins(0, 0, 0, 0);

    this->xmlFile = "";
    this->doesUseXMLFile = false;

    QFile xmlConfig ("xmlconfig.cfg");
    if (!QFile::exists("xmlconfig.cfg")) {
        if (xmlConfig.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&xmlConfig);
            out << "\n0";
            xmlConfig.close();
        }
    } else {
        if (xmlConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&xmlConfig);
            int i = 0;
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (i == 0) {
                    this->xmlFile = line;
                } else if (i == 1) {
                    this->doesUseXMLFile = line.toInt() != 0;
                }
                i++;
            }
            xmlConfig.close();
        }
    }


    QFile recents ("recents.cfg");
    if (!QFile::exists("recents.cfg")) {
        if (recents.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&recents);
            out << "\n\n\n\n";
            recents.close();
        }
    } else {
        if (recents.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&recents);
            while (!in.atEnd()) {
                this->recentFiles << in.readLine();
            }
            recents.close();
        }
    }

    while (this->recentFiles.count() < 5) {
        this->recentFiles << "";
    }

    axinterpreter = new AXInterpreter(this);

    QSplitter* mainSplitter = new QSplitter(this);
    mainSplitter->setContentsMargins(0, 0, 0, 0);
    mainSplitter->setHandleWidth(5);
    mainSplitter->setChildrenCollapsible(false);

    sideBarView = new SideBarView(this);
    sideBarView->setContentsMargins(0, 0, 0, 0);
    sideBarView->setTheme(getTheme("theme.cfg"));

    detailView = new DetailView(this);
    detailView->setContentsMargins(0, 0, 0, 0);

    mainSplitter->addWidget(sideBarView);
    mainSplitter->addWidget(detailView);

    mainSplitter->setSizes(QList<int>() << 200 << 480-200);

    dummyLayout->addWidget(mainSplitter);
    this->setLayout(dummyLayout);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(obtainScriptFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O + Qt::SHIFT), this, SLOT(askToOpenCSVDir()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(saveScriptFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S + Qt::SHIFT), this, SLOT(saveScriptFileAs()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(askToCompile()));
}



void ContentView::setTheme(int idClicked) {
    this->sideBarView->setTheme((Theme) idClicked);
}


void ContentView::obtainScriptFile() {

    QString newScriptFile = QFileDialog::getOpenFileName(this, tr("Select script file"));

    this->readScriptFile(newScriptFile);

    this->shiftRecentFiles();
}

void ContentView::obtainXMLFile() {
    QString newXMLFile = QFileDialog::getOpenFileName(this, tr("Select .XML file"));

    qDebug() << newXMLFile;

    if (!newXMLFile.count()) { return; }

    this->xmlFile = newXMLFile;
    emit this->xmlFileObtained(this->xmlFile);
    this->saveXMLConfig();
}



void ContentView::saveScriptFile() {

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
    file.open(scriptFile.toStdString().c_str());

    // Error handling
    if (!file.is_open()) {
        qDebug() << "Error writing to file";
        return;
    }

    // Saving Theme to file
    file << (this->detailView->editorView->textEdit->document()->toPlainText().toStdString().c_str());
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

    this->shiftRecentFiles();
}



void ContentView::shiftRecentFiles() {

    if (!this->recentFiles.contains(scriptFile)) {
        recentFiles.removeLast();
        recentFiles.prepend(scriptFile);
    }

    emit this->recentFilesChanged();

    QFile recents ("recents.cfg");
    if (recents.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out (&recents);
        out << this->recentFiles.join("\n");
        recents.close();
    }
}



void ContentView::readScriptFile(QString newScriptFile) {
    if (newScriptFile == "") { return; }

    QFileInfo fileInfo(newScriptFile);
    QMessageBox messageBox;

    const QString fileName = fileInfo.fileName();

    if (!QFileInfo::exists(newScriptFile)) {
        messageBox.setText("File name \"" + fileName + "\" does not exist.");
        messageBox.exec();
        return;
    }

    if (fileName.count('.') > 1) {
        messageBox.setText("File name \"" + fileName + "\" contains more than one '.' character. Unable to read file.");
        messageBox.exec();
        return;
    }

    if (fileName.count(' ') > 0) {
        messageBox.setText("File name \"" + fileName + "\" contains a whitespace character. Unable to read file.");
        messageBox.exec();
        return;
    }

    this->scriptFile = newScriptFile;
    this->sideBarView->setFileLabel(scriptFile);
    this->detailView->editorView->readRecipeFile(scriptFile);
    this->detailView->tabView->setCurrentIndex(0);
}



void ContentView::askToCompile() {
    this->axinterpreter->startCompilation(this->scriptFile);
}

void ContentView::refreshInferredVariables() {
    this->sideBarView->inferredVariablesView->refreshVariables();
    this->detailView->stackedGraphsView->replot();
}


void ContentView::askToOpenCSVDir() {
    QString dirName = QFileDialog::getExistingDirectory(this, "Select a result directory");
    this->openCSVDir(dirName);
}


void ContentView::openCSVDir(QString dirName) {
    this->axinterpreter->loadResultsFrom(dirName);
}

void ContentView::saveXMLConfig() {
    QFile xmlConfig ("xmlconfig.cfg");
    if (xmlConfig.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out (&xmlConfig);
        out << this->xmlFile << "\n" << QString::number(int (this->doesUseXMLFile));
        xmlConfig.close();
    }
}

