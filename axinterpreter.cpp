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
#include "convertaxmtopy.h"

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

    // Generete python script file
    this->mainText->setText("Creating python script file");
    if (this->generatePyFile()) {
        this->mainText->setText("Failed to create main.py");
        return;
    }

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
        // Add all lines next to one another (no newline)
        result += line + " ";
    }

    // Regex to find macro definitions
    // Match when a single non-digit, non-space char is followed by any amount of word chars, a '{', anything, '}'
    // Catches: entire definition (0), macro name (1), macro contents (2)
    QRegularExpression regexDefinitions ("([^\\s\\d][\\w]*)\\s*[{]([^}]*)[}]");
    while (1) {
        QRegularExpressionMatch matchDefinitions = regexDefinitions.match(result);

        // if there is a macro declaration
        if (matchDefinitions.hasMatch()) {

            // remove the declaration
            result.remove(matchDefinitions.captured(0));

            // replace all macro calls (captured(1)) with macro contents (captured(2))
            result.replace(" " + matchDefinitions.captured(1) + ";", " " + matchDefinitions.captured(2));
        } else {
            // Stop the loop once there are no macro definitions left
            break;
        }
    }

    // Regex to find dots in non-digits
    // Match when a non-digit is followed by a dot
    // Capture both chars (0) and the first char (1)
    QRegularExpression regexDot ("(\\D)[\\.]");
    while (1) {
        QRegularExpressionMatch matchDot = regexDot.match(result);

        // if there is a dot in a non-digit
        if (matchDot.hasMatch()) {
            // Replace the sequence (char + dot) with the first character and an underscore
            result.replace(matchDot.captured(0), matchDot.captured(1) + "_");
        } else {
            // Stop the loop once there are no dots to replace
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


int AXInterpreter::generatePyFile() {
    // Create file header
    QFile axmFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axm");
    QFile pyFileHeader(this->baseFolder.absolutePath() + QDir::separator() + "main.py");

    // Try opening the files
    if (!(axmFileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return -1;
    }

    if (!(pyFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return -2;
    }

    // Create text streams (in from .AXM, out to .py)
    QTextStream in (&axmFileHeader);
    QTextStream out (&pyFileHeader);

    // Text to write to .py
    QString result = QString();

    // Add a default header to always be created
    result += "import time\n";
    result += "import threading\n\n";
    result += "on = 1\n";
    result += "off = 1\n";
    result += "open = 1\n";
    result += "closed = 0\n\n";

    // Read from .AXM
    while (!(in.atEnd())) {
        QString line = in.readLine();
        result += convertAXMtoPy(line) + "\n";
    }

    out << result;

    axmFileHeader.close();
    pyFileHeader.close();

    return 0;
}
