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

    this->mainText = new QLabel("Test");
    this->setCentralWidget(this->mainText);
    this->hide();
}



void AXInterpreter::startCompilation(QString scriptFile) {

    this->show();

    // Check if the given scriptFile is valid
    if (!scriptFile.count()) {
        qDebug() << "No file specified!";
        this->mainText->setText("No file specified!");
        return;
    }

    if (!QFileInfo(scriptFile).exists()) {
        qDebug() << "Specified file does not exist!";
        this->mainText->setText("Specified file does not exist!");
        return;
    }

    this->scriptFile = scriptFile;

    if (this->recreateFolder()) {
        qDebug() << "Failed to recreate folder!";
        this->mainText->setText("Failed to recreate folder!");
        return;
    }

    qDebug() << "Folder " << this->baseFolder.absolutePath() << " created!";
    this->mainText->setText("Folder " + this->baseFolder.absolutePath() + " created!");

    // Create an .AXR file (READ commands replaced with contents of specified files)
    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axr...");

    // Create an .AXC file (Comments removed)
    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axc...");

    // Create an .AXS file (Separated by semicolons)
    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axs...");


    // Infer variables

    // Create .AXV files (Per-variable script files)

    // Create .CSV files for plotting

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
