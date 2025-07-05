#include <QDebug>
#include <QLabel>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextStream>

#include "axinterpreter.h"
#include "contentview.h"
#include "recursiveReplaceRead.h"

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
    if (this->generateAXRfile()) {
        this->mainText->setText("Failed to create " + this->baseFolder.dirName() + ".axr");
        return;
    }

    // Create an .AXC file (Comments removed)
    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axc...");
    if (this->generateAXCfile()) {
        this->mainText->setText("Failed to create " + this->baseFolder.dirName() + ".axc");
        return;
    }

    // Create an .AXM file (Macros pasted in place of their call functions)
    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axm...");
    if (this->generateAXMfile()) {
        this->mainText->setText("Failed to create " + this->baseFolder.dirName() + ".axm");
        return;
    }

    // Infer variables

    // Create .AXV files (Per-variable script files)

    // Create .CSV files for plotting

    // Done!
    this->mainText->setText("Done!");

}


int AXInterpreter::recreateFolder() {
    // Create a new folder named after the given scriptFile
    QFileInfo fileInfo(this->scriptFile);
    QString baseFolderName = fileInfo.absolutePath() + QDir::separator() + fileInfo.completeBaseName();
    QString scriptFolderName = fileInfo.absolutePath();

    this->baseFolder = QDir(baseFolderName);
    this->scriptFileFolder = QDir(scriptFolderName);

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


int AXInterpreter::generateAXRfile() {

    // Create the .AXR file header
    QFile axrFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axr");

    // Create and open the .AXR file
    if (!(axrFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return -1;
    }

    // Write text to file
    QTextStream out (&axrFileHeader);
    out << recursiveReplaceRead(this->scriptFile);

    // Close the file
    axrFileHeader.close();

    return 0;
}


int AXInterpreter::generateAXCfile() {
    // Create the .AXC file header
    QFile axcFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axc");

    // Create and open the .AXC file
    if (!(axcFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return -1;
    }

    // Create the .AXR file header
    QFile axrFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axr");

    // Open the .AXR file (Read-only)
    if(!(axrFileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return -2;
    }

    // Create the text streams for reading and writing
    QTextStream in (&axrFileHeader);
    QTextStream out (&axcFileHeader);

    // Variable to store the result of reading from .AXR
    QString result = QString();

    // Regex to find comments (from '#' to the end of the line)
    // (Match if has at least 0 chars that are not '#' followed by a '#' and at least 0 of any other char)
    // Also capture everything proceeding the '#' char
    QRegularExpression regexComment("^([^#]*)#.*$");

    // Read the .AXR file
    while (!(in.atEnd())) {
        QString line = in.readLine();
        QRegularExpressionMatch matchComment = regexComment.match(line);

        if (matchComment.hasMatch()) {
            line = matchComment.captured(1);
        }
        // Remove whitespaces from beginning and the end of the line, remove multiple whitespaces
        line = line.simplified();

        // Skip empty lines
        if (line != QString()) {
            // Add all lines next to one another (no newline)
            result += line;
        }
    }

    // Regenerate whitespaces
    result.replace(",", ", ");
    result.replace(";", ";\n");
    result.replace("{", "{\n");
    result.replace("}", "}\n");

    result += "\nlayer;";

    // Write to the .AXC
    out << result;

    // Close all files
    axrFileHeader.close();
    axcFileHeader.close();

    return 0;

}


int AXInterpreter::generateAXMfile() {

    // Create the .AXM file header
    QFile axmFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axm");

    // Create and open the .AXM file
    if (!(axmFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return -1;
    }

    // Create the .AXC file header
    QFile axcFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axc");

    // Open the .AXC file (Read-only)
    if(!(axcFileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return -2;
    }

    // Create the text streams for reading and writing
    QTextStream in (&axcFileHeader);
    QTextStream out (&axmFileHeader);

    // Variable to store the result of reading from .AXC
    QString result = QString();
    while (!(in.atEnd())) {
        QString line = in.readLine().trimmed();
        result += line + " ";
    }

    // Regex to find macro definitions
    QRegularExpression regexDefinitions ("([^\\s\\d][\\w]*)\\s*[{]([^}]*)[}]");
    while (1) {
        QRegularExpressionMatch matchDefinitions = regexDefinitions.match(result);
        if (matchDefinitions.hasMatch()) {
            result.remove(matchDefinitions.captured(0));
            result.replace(" " + matchDefinitions.captured(1) + ";", " " + matchDefinitions.captured(2));
        } else {
            break;
        }
    }

    // Regenerate whitespaces
    result = result.simplified();
    result.replace("; ", ";\n");
    result.replace(" ;", ";");

    // Remove unresolved macros
    result.remove(QRegularExpression("\n[^\\d\\s][\\w]*[;]"));

    // Write to .AXM file
    out << result;

    // Close files
    axcFileHeader.close();
    axmFileHeader.close();

    return 0;
}
