#include <QDebug>
#include <QLabel>
#include <QDir>
#include <QFileInfo>

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



void AXInterpreter::startCompilation(QString scriptFile) {


    // Check if the given scriptFile is valid
    if (!scriptFile.count()) {
        qDebug() << "No file specified!";
        return;
    }

    this->scriptFile = scriptFile;

    if (!this->recreateFolder()) {
        qDebug() << "recreateFolder() failed!";
        return;
    }

    this->show();
}

int AXInterpreter::recreateFolder() {
    // Create a new folder named after the given scriptFile
    QFileInfo fileInfo(this->scriptFile);
    QString baseFolderName = fileInfo.absolutePath() + QDir::separator() + fileInfo.completeBaseName();

    this->baseFolder = QDir(baseFolderName);

    // Delete the folder and all of its contents
    if (baseFolder.exists()) {
        qDebug() << "Folder does exist. deleting...";
        if (!baseFolder.removeRecursively()) {
            qDebug() << "Failed to remove.";
            return -1;
        }
        qDebug() << "Removed!!";
    }

    // Recreate the folder
    qDebug() << "Folder does not exist. creating...";
    if (!baseFolder.mkpath(".")) {
        qDebug() << "Failed to create.";
        return -2;
    }
    qDebug() << "Created!!";
    return 0;
}
