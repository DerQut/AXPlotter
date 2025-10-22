#include <QDebug>
#include <QLabel>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextStream>
#include <QProcess>
#include <QThread>

#include "axinterpreter.h"
#include "contentview.h"
#include "detailview.h"
#include "graphsview.h"
#include "recursiveReplaceRead.h"
#include "convertaxmtopy.h"

AXInterpreter::AXInterpreter(ContentView* parent) :
    QMainWindow(parent)
{

    this->contentView = parent;

    this->mainText = new QLabel("Test");
    this->setCentralWidget(this->mainText);
    this->hide();

    this->setWindowTitle("AX Interpreter");

}



void AXInterpreter::startCompilation(QString scriptFile) {

    // Remove all plots
    this->contentView->detailView->graphsView->deletePlots();

    // Show the popup window
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
    QString axmResult = this->generateAXMfile();
    if (!axmResult.isEmpty()) {
        this->mainText->setText(axmResult);
        return;
    }

    // Generete python script file
    this->mainText->setText("Creating python script file");
    if (this->generatePyFile()) {
        this->mainText->setText("Failed to create main.py");
        return;
    }

    // Create .CSV files for plotting
    this->mainText->setText("Running main.py");
    QString stdErrors = this->launchPy();
    if (stdErrors != "") {
        this->mainText->setText("Error encountered when running the generated script:\n" + stdErrors);
    } else {
        // Done!
        this->mainText->setText("Done!");
    }

    this->contentView->detailView->graphsView->updatePlots(this->baseFolder.absolutePath());
    this->contentView->detailView->tabView->setCurrentIndex(1);
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

    // Write to the .AXC
    out << result;

    // Close all files
    axrFileHeader.close();
    axcFileHeader.close();

    return 0;

}


QString AXInterpreter::generateAXMfile() {

    // Create the .AXM file header
    QFile axmFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axm");

    // Create and open the .AXM file
    if (!(axmFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return "Failed to create file " +(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName())+ ".axm";
    }

    // Create the .AXC file header
    QFile axcFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axc");

    // Open the .AXC file (Read-only)
    if(!(axcFileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return "Failed to read file " +(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName())+ ".axc";
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

    // Close the file
    axcFileHeader.close();

    // Find macro definitions
    while (true) {

        // Find the start of a macro definition ("macro_name {")
        // Excludes word "layer" and "loop"
        QRegularExpression regexDefStart("\\b(?![lL][oO][oO][pP]\\b|\\b[lL][aA][yY][eE][rR]\\b)([^\\s\\d;{}()][\\w]*)\\s*\\{");

        QRegularExpressionMatch defStartMatch = regexDefStart.match(result);

        if (!defStartMatch.hasMatch()) {
            // All macro definitions found
            break;
        }

        QString macroName = defStartMatch.captured(1);
        int openBracePos = defStartMatch.capturedEnd(0) - 1;

        int closeBracePos = findMatchingBrace(result, openBracePos);

        if (closeBracePos == -1) {
            axmFileHeader.close();
            return "Syntax error: Unmatched brace for block: " +macroName;
        }

        // Extract the macro content and the full definition text.
        int contentStart = openBracePos + 1;
        int contentLength = closeBracePos - contentStart;
        QString macroContent = result.mid(contentStart, contentLength);

        int defStart = defStartMatch.capturedStart(0);
        int defLength = (closeBracePos + 1) - defStart;

        // Remove the macro definition
        result.remove(defStart, defLength);

        // Replace the macro call with their content
        QRegularExpression regexCall("\\b" + QRegularExpression::escape(macroName) + "\\b\\s*;");
        result.replace(regexCall, macroContent);
    }

    // Replace all the code with the contents of the Layer block
    QRegularExpression regexLayerStart("\\b[lL][aA][yY][eE][rR]\\b\\s*\\{");
    QRegularExpressionMatch layerStartMatch = regexLayerStart.match(result);

    if (result.count(regexLayerStart) > 1) {
        axmFileHeader.close();
        return "Syntax error: multiple Layer macros defined!";
    }

    if (layerStartMatch.hasMatch()) {
        int layerStartBlock = layerStartMatch.capturedStart(0);
        int layerOpenBrace = layerStartMatch.capturedEnd(0) - 1;
        int layerCloseBrace = findMatchingBrace(result, layerOpenBrace);

        if (layerCloseBrace != -1) {
            int contentStart = layerOpenBrace + 1;
            int contentLength = layerCloseBrace - contentStart;
            QString layerContent = result.mid(contentStart, contentLength);

            // Extract the content inside the main block.
            result.replace(layerStartMatch.capturedStart(0), layerCloseBrace + 1 - layerStartBlock, layerContent);
        } else {
            axmFileHeader.close();
            return "Syntax error: Layer block not closed";
        }
    } else {
        axmFileHeader.close();
        return "Syntax error: Layer block not found";
    }

    // Regex to find dots in non-digits
    // Match when a dot is followed by a non-digit
    // Capture both chars (0) and the last char (1)
    QRegularExpression regexDot ("[\\.]([^\\d])");
    result.replace(regexDot, "_\\1");

    // Replace "parameter" keyword with "variable"
    result.replace(QRegularExpression("\\b[pP][aA][rR][aA][mM][eE][tT][eE][rR]\\b"), " variable ");

    // Replace keyword values with a caps-locked version
    result.replace(QRegularExpression("\\b[oO][pP][eE][nN]\\b"), "OPEN");
    result.replace(QRegularExpression("\\b[cC][lL][oO][sS][eE]\\b"), "CLOSE");
    result.replace(QRegularExpression("\\b[oO][nN]\\b"), "ON");
    result.replace(QRegularExpression("\\b[oO][fF][fF]\\b"), "OFF");

    // Regenerate whitespaces
    result = result.simplified();
    result.replace("; ", ";\n");
    result.replace(" ;", ";");
    result.replace("{", "{\n");
    result.replace("}", "}\n");

    // Remove unresolved macros
    result.remove(QRegularExpression("\n[^\\d\\s][\\w]*[;]"));

    // Remove messages ("message")
    result.remove(QRegularExpression("\"[^\"]*\","));

    // Write to .AXM file
    out << result;

    // Close the file
    axmFileHeader.close();

    return "";
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

    // Number of tabs to insert (used in loops)
    int tabCount = 0;

    // Add a default header to always be created
    result += "from math import sin, cos, tan, exp, log, log10, sqrt\n\n";
    result += "ON = 1\n";
    result += "OFF = 1\n";
    result += "OPEN = 1\n";
    result += "CLOSE = 0\n\n";
    result += "def exp10(x):\n";
    result += "    return 10**x\n\n";
    result += "AX_GLOBAL_TIMESTEP = 0\n\n";

    // Regular expressions for finding beginnings and endings of loops
    QRegularExpression regexLoopStart (".*\\#AXLOOPSTART\\$.*");
    QRegularExpression regexLoopEnd (".*\\#AXLOOPEND\\$.*");

    // Read from .AXM
    while (!(in.atEnd())) {
        QString line = in.readLine();

        // Force python syntax for less than, greater than, pow
        line = line.replace(QRegularExpression("<<"), "<");
        line = line.replace(QRegularExpression(">>"), ">");
        line = line.replace(QRegularExpression("\\^"), "**");

        QString pyLine = convertAXMtoPy(line);
        QStringList pyLines = pyLine.split("\n");

        for (int i = 0; i < pyLines.count(); i++) {
            QRegularExpressionMatch matchLoopStart = regexLoopStart.match(pyLines.at(i));
            QRegularExpressionMatch matchLoopEnd = regexLoopEnd.match(pyLines.at(i));

            for (int j = 0; j < tabCount; j++) {
                result += "    ";
            }

            if (matchLoopStart.hasMatch()) {
                tabCount++;
            }

            if (matchLoopEnd.hasMatch() && tabCount) {
                tabCount--;
            }

            result += pyLines.at(i) + "\n";
        }
    }

    // Add a command to register the max timestep
    result += "\nfile = open(\"timestep.txt\", \"w+\")";
    result += "\nfile.write(str(AX_GLOBAL_TIMESTEP))";
    result += "\nfile.close()";

    // Write to .py file
    out << result;

    // Close both files
    axmFileHeader.close();
    pyFileHeader.close();

    return 0;
}


QString AXInterpreter::launchPy() {
    QDir::setCurrent(this->baseFolder.absolutePath());
    QProcess pyProcess;
    pyProcess.setProgram("cmd.exe");
    pyProcess.setArguments(QStringList() << "/C" << "python main.py");
    pyProcess.start();
    pyProcess.waitForFinished(-1);

    QString stdOutput = pyProcess.readAllStandardOutput();
    QString stdErrors = pyProcess.readAllStandardError();

    qDebug() << "Output:" << stdOutput;
    qDebug() << "Errors:" << stdErrors;

    pyProcess.kill();

    QDir::setCurrent("C:/");

    return stdErrors;
}

int findMatchingBrace(const QString& str, int startPos) {
    if (startPos < 0 || startPos >= str.length() || str[startPos] != '{') {
        // Invalid starting position
        return -1;
    }

    int depth = 1;
    for (int i = startPos + 1; i < str.length(); ++i) {
        // if finds another '{', then the next one is not the closing brace
        if (str[i] == '{') {
            depth++;
        } else if (str[i] == '}') {
            depth--;
            if (depth == 0) {
                // Matching brace found
                return i;
            }
        }
    }
    // Matching brace not found
    return -1;
}
