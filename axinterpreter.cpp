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
#include <QXmlStreamReader>

#include "axinterpreter.h"
#include "contentview.h"
#include "detailview.h"
#include "graphsview.h"
#include "recursiveReplaceRead.h"
#include "convertaxmtopy.h"
#include "stackedgraphsview.h"
#include "qcustomplot/qcustomplot.h"


AXInterpreter::AXInterpreter(ContentView* parent) :
    QMainWindow(parent)
{

    this->contentView = parent;

    this->mainText = new QLabel("Test");
    this->setCentralWidget(this->mainText);
    this->hide();

    this->setWindowTitle("AX Interpreter");

    this->mainText->setContentsMargins(10, 10, 10, 10);

}



void AXInterpreter::startCompilation(QString scriptFile) {

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
    QString axrResult = this->generateAXRfile();

    this->mainText->setText("Creating " + this->baseFolder.dirName() + ".axr...");
    QRegularExpression regexAXRError ("\\$AXR_ERROR_START([^\\$]*)\\$AXR_ERROR_END");
    QString error = "";
    while (true) {
        QRegularExpressionMatch matchAXRError = regexAXRError.match(axrResult);
        if (matchAXRError.hasMatch()) {
            error += matchAXRError.captured(1) + "\n";
            axrResult.remove(matchAXRError.captured(0));
        } else {
            break;
        }
    }

    if (!error.isEmpty()) {
        this->mainText->setText(error);
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

    this->loadResults();
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


QString AXInterpreter::generateAXRfile() {
    // Create the .AXR file header
    QFile axrFileHeader(this->baseFolder.absolutePath() + QDir::separator() + this->baseFolder.dirName() + ".axr");

    // Create and open the .AXR file
    if (!(axrFileHeader.open(QIODevice::WriteOnly | QIODevice::Text))) {
        return "$AXR_ERROR_START Failed to create " +this->baseFolder.dirName()+ ".axr $AXR_ERROR_END";
    }

    // Write text to file
    QTextStream out (&axrFileHeader);
    QString returnVal = "";

    if (this->contentView->doesUseXMLFile) {
        QStringList deviceCodes;
        QStringList deviceNewNames;

        QRegularExpression regexOtherDevice ("^(\\D[\\d]+)$");
        QStringList badCategories = {"Materials", "ALARM", "CALCULATIONS"};
        QStringList alreadyWrittenNames;

        for (int i = 0; i < 2; i++) {
            QFile xmlFile(this->contentView->xmlFile);

            if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                axrFileHeader.close();
                return "$AXR_ERROR_START Failed to read " +this->contentView->xmlFile+ " $AXR_ERROR_END";
            }

            QXmlStreamReader xmlReader(&xmlFile);

            bool isInsideDeviceBlock = false;

            QString deviceCode = "";
            QString deviceName = "";
            QString deviceDef = "";
            QString deviceMin = "0";
            QString deviceMax = "1";

            while (!xmlReader.atEnd()) {
                xmlReader.readNext();

                if (xmlReader.isStartElement()) {
                    if (xmlReader.name().toString() == "DEVICE" | xmlReader.name().toString() == "NODE") {
                        deviceCode = xmlReader.attributes().value("NAME").toString();

                        if (deviceCodes.contains(deviceCode) || i == 0) {
                            isInsideDeviceBlock = true;
                            deviceName = "";
                            deviceDef = "0";
                            deviceMin = "";
                            deviceMax = "";
                        }

                    } else if (isInsideDeviceBlock && xmlReader.name().toString() == "PROPERTY") {
                        QString propName = xmlReader.attributes().value("NAME").toString().toLower();

                        QString elementText = xmlReader.readElementText();

                        if (!elementText.count()) {
                            continue;
                        }

                        QRegularExpressionMatch matchOtherDevice = regexOtherDevice.match(elementText);

                        if (propName == "category" && (badCategories.contains(elementText))) {
                            // skip this device
                            isInsideDeviceBlock = false;
                            continue;

                        } else if (propName == "username") {
                            deviceName = elementText;

                        } else if (propName == "default" || propName == "physdef") {
                            deviceDef = elementText;

                        } else if (propName == "physmin") {
                            deviceMin = elementText;

                        } else if (propName == "physmax") {
                            deviceMax = elementText;

                        } else if (matchOtherDevice.hasMatch()) {
                            deviceCodes.append(matchOtherDevice.captured(1));

                            QString newName = deviceName+ "_" +propName.toLower();
                            deviceNewNames.append(newName);

                        }

                        if (i && deviceCodes.contains(deviceCode)) {
                            int j = deviceCodes.indexOf(deviceCode);
                            deviceName = deviceNewNames.at(j);
                        }

                    }

                } else if (xmlReader.isEndElement() && (xmlReader.name().toString() == "DEVICE" | xmlReader.name().toString() == "NODE")) {
                    isInsideDeviceBlock = false;

                    if (!deviceName.isEmpty() && !deviceName.contains("@")) {

                        deviceName.replace(QRegularExpression("\\."), "_");

                        if (!alreadyWrittenNames.contains(deviceName)) {
                            returnVal += "\nvariable " + deviceName + " = " + deviceDef + ";\n";
                            returnVal += "#$" + deviceName + ".physDef = " + deviceDef + "#;\n";

                            if (!deviceMin.isEmpty())
                                returnVal += "#$" + deviceName + ".physMin = " + deviceMin + "#;\n";

                            if (!deviceMax.isEmpty())
                                returnVal += "#$" + deviceName + ".physMax = " + deviceMax + "#;\n";
                        }

                        alreadyWrittenNames << deviceName;
                    }
                }

            }

            xmlFile.close();
        }
    }

    QStringList* fileNames = new QStringList();

    returnVal += recursiveReplaceRead(this->scriptFile, fileNames);
    delete fileNames;

    QStringList iterableProps = {"push", "inject", "run"};
    for (int i=0; i < iterableProps.count(); i++) {
        QRegularExpression regexIterable ("[\\.\\_]" +iterableProps[i]+ "\\b");
        while (true) {
            QRegularExpressionMatch matchIterable = regexIterable.match(returnVal);
            if (!matchIterable.hasMatch()) {
                break;
            }
            returnVal.replace(regexIterable, "_"+iterableProps[i]+"1");
        }
    }

    out << returnVal;

    // Close the file
    axrFileHeader.close();

    return returnVal;
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
    QRegularExpression regexComment ("^([^#]*)#.*$");

    QRegularExpression regexPythonCall ("^#\\$.*\\#;$");

    // Read the .AXR file
    while (!(in.atEnd())) {
        QString line = in.readLine();
        QRegularExpressionMatch matchComment = regexComment.match(line);
        QRegularExpressionMatch matchPythonCall = regexPythonCall.match(line);

        if (matchComment.hasMatch() && !matchPythonCall.hasMatch()) {
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

    QString returnVal = QString("");

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
        QRegularExpression regexMacroStart("\\b(?![l][o][o][p]\\b|\\b[l][a][y][e][r]\\b)([^\\s\\d\\.;{}()][\\w]*)\\s*\\{");

        QRegularExpressionMatch matchMacroStart = regexMacroStart.match(result);

        if (!matchMacroStart.hasMatch()) {
            // All macro definitions found
            break;
        }

        QString macroName = matchMacroStart.captured(1);
        int openBracePos = matchMacroStart.capturedEnd(0) - 1;

        int closeBracePos = findMatchingBrace(result, openBracePos);

        if (closeBracePos == -1) {
            axmFileHeader.close();
            return "Syntax error: Unmatched brace for block: " +macroName;
        }

        // Extract the macro content and the full definition text.
        int contentStart = openBracePos + 1;
        int contentLength = closeBracePos - contentStart;
        QString macroContent = result.mid(contentStart, contentLength);

        int macroStart = matchMacroStart.capturedStart(0);
        int macroLength = (closeBracePos + 1) - macroStart;

        // Remove the macro definition
        result.remove(macroStart, macroLength);

        // Replace the macro call with their content
        QRegularExpression regexCall("\\b" + QRegularExpression::escape(macroName) + "\\b\\s*;");
        result.replace(regexCall, macroContent);
    }

    // Replace all the code with the contents of the Layer block
    const QRegularExpression regexLayerStart("\\b[l][a][y][e][r]\\b\\s*\\{");
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

    // Regenerate whitespaces
    result = result.simplified();
    result.replace("; ", ";\n");
    result.replace(" ;", ";");
    result.replace("{", "{\n");
    result.replace("}", "}\n");
    result.replace(QRegularExpression("\n[\\ ]*"), "\n");

    const QStringList resultSplit = result.split("\n");
    const QRegularExpression regexPythonCall ("^\\s*#\\$(.*)\\#;\\s*$");

    // Regex to find dots in non-digits
    // Match when a dot is followed by a non-digit
    // Capture both chars (0) and the last char (1)
    const QRegularExpression regexDot ("[\\.]([^\\d])");

    result = "";

    for (int i = 0; i < resultSplit.count(); i++) {

        QString line = resultSplit.at(i);
        QRegularExpressionMatch matchPythonCall = regexPythonCall.match(line);

        if (!matchPythonCall.hasMatch()) {
            line.replace(regexDot, "_\\1");
        }
        result += line + "\n";
    }

    // Replace "parameter" keyword with "variable"
    result.replace(QRegularExpression("\\bparameter\\b"), " variable ");

    // Replace keyword values with a caps-locked version
    result.replace(QRegularExpression("\\bopen\\b"), "OPEN");
    result.replace(QRegularExpression("\\bclose\\b"), "CLOSE");
    result.replace(QRegularExpression("\\bon\\b"), "ON");
    result.replace(QRegularExpression("\\boff\\b"), "OFF");

    //remove "begin stat" and "end stat" calls
    result.replace(QRegularExpression("\\bbegin stat[^\\;]*\\;"), ";");
    result.replace(QRegularExpression("\\bend stat[^\\;]*\\;"), ";");

    // Remove messages ("message")
    result.remove(QRegularExpression("\"[^\"]*\","));

    // Remove unresolved macros
    const QRegularExpression regexUnresolvedMacro("^(?![l][o][o][p]\\b|\\b[l][a][y][e][r]\\b)([^\\s\\d\\.;{}()][\\w]*)\\s*\\;");

    while (true) {
        QRegularExpressionMatch matchUnresolvedMacro = regexUnresolvedMacro.match(result);
        if (matchUnresolvedMacro.hasMatch()) {
            returnVal += "Error: unresolved macro: \"" +matchUnresolvedMacro.captured(1)+ "\"\n";
            result.remove(matchUnresolvedMacro.captured(0));
        } else {
            break;
        }
    }

    result.remove(QRegularExpression(regexUnresolvedMacro));

    // Write to .AXM file
    out << result;

    // Close the file
    axmFileHeader.close();

    return returnVal;
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
    result += "import sys\n";
    result += "import types\n";
    result += "from math import sin, cos, tan, exp, log, log10, sqrt\n\n";

    result += "AX_GLOBAL_TIMESTEP = 0\n\n";

    result += "class AXVariable:\n";
    result += "    all=[]\n";
    result += "    def __init__(self, name):\n";
    result += "        self.name = name\n";
    result += "        self.physMin = 0\n";
    result += "        self.physMax = 0\n";
    result += "        self.physDef = 0\n\n";

    result += "        self.usesMin = False\n";
    result += "        self.usesMax = False\n\n";

    result += "        self.currentValue = 0\n";
    result += "        self.endValue = 0\n\n";

    result += "        self.followLambda = None\n";
    result += "        AXVariable.all.append(self)\n\n";

    result += "    def __int__(self):\n";
    result += "        return int(self.currentValue)\n\n";

    result += "    def __float__(self):\n";
    result += "        return float(self.currentValue)\n\n";

    result += "    def __add__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue + other.currentValue\n";
    result += "        return self.currentValue + other\n\n";

    result += "    def __radd__(self, other):\n";
    result += "        return self.__add__(other)\n\n";

    result += "    def __sub__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue - other.currentValue\n";
    result += "        return self.currentValue - other\n\n";

    result += "    def __rsub__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return other.currentValue - self.currentValue\n";
    result += "        return other - self.currentValue\n\n";

    result += "    def __mul__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue * other.currentValue\n";
    result += "        return self.currentValue * other\n\n";

    result += "    def __rmul__(self, other):\n";
    result += "        return self.__mul__(other)\n\n";

    result += "    def __pow__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue ** other.currentValue\n";
    result += "        return self.currentValue ** other\n\n";

    result += "    def __rpow__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return other.currentValue ** self.currentValue\n";
    result += "        return other ** self.currentValue\n\n";

    result += "    def __truediv__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue / other.currentValue\n";
    result += "        return self.currentValue / other\n\n";

    result += "    def __rtruediv__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return other.currentValue / self.currentValue\n";
    result += "        return other / self.currentValue\n\n";

    result += "    def __lt__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue < other.currentValue\n";
    result += "        return self.currentValue < other\n\n";

    result += "    def __le__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue <= other.currentValue\n";
    result += "        return self.currentValue <= other\n\n";

    result += "    def __gt__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue > other.currentValue\n";
    result += "        return self.currentValue > other\n\n";

    result += "    def __ge__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue >= other.currentValue\n";
    result += "        return self.currentValue >= other\n\n";

    result += "    def __eq__(self, other):\n";
    result += "        if isinstance(other, AXVariable):\n";
    result += "            return self.currentValue == other.currentValue\n";
    result += "        return self.currentValue == other\n\n";

    result += "    def setPhysMin(self, physMin):\n";
    result += "        self.physMin = physMin\n";
    result += "        self.usesMin = True\n\n";

    result += "    def setPhysMax(self, physMax):\n";
    result += "        self.physMax = physMax\n";
    result += "        self.usesMax = True\n\n";

    result += "    def setCurrentValue(self, value, forcedByFollower=False):\n";
    result += "        global AX_GLOBAL_TIMESTEP\n\n";

    result += "        self.currentValue = float(value)\n\n";

    result += "        if self.usesMin and self.currentValue < self.physMin:\n";
    result += "            self.currentValue = self.physMin\n\n";

    result += "        if self.usesMax and self.currentValue > self.physMax:\n";
    result += "            self.currentValue = self.physMax\n\n";

    result += "        file = open(self.name+\".csv\", \"a+\")\n";
    result += "        file.write(str(AX_GLOBAL_TIMESTEP) + ',' + str(self.currentValue) + '\\n')\n\n";

    result += "        if not forcedByFollower:\n";
    result += "            self.followLambda = None\n\n";

    result += "            for axvar in AXVariable.all:\n";
    result += "                axvar.calculateFollower()\n\n";

    result += "    def calculateFollower(self):\n";
    result += "        if (isinstance(self.followLambda, types.FunctionType) and self.followLambda.__name__ == \"<lambda>\"):\n";
    result += "            self.setCurrentValue(self.followLambda(), True)\n";
    result += "            self.endValue = self.currentValue\n\n";

    result += "ON = 1\n";
    result += "OFF = 0\n";
    result += "OPEN = 1\n";
    result += "CLOSE = 0\n\n";

    result += "def exp10(x):\n";
    result += "    return 10**x\n\n";

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


void AXInterpreter::loadResultsFrom(QString dirName) {
    this->baseFolder = QDir(dirName);

    if (!this->baseFolder.exists()) {
        qDebug() << "Result folder " << this->baseFolder.absolutePath() << " does not exist!";
        return;
    }

    this->loadResults();
}

void AXInterpreter::loadResults() {
    // Remove all plots
    this->contentView->detailView->graphsView->deletePlots();
    this->contentView->detailView->stackedGraphsView->plot->clearPlottables();
    this->contentView->detailView->stackedGraphsView->plot->replot();

    this->contentView->detailView->graphsView->updatePlots(this->baseFolder.absolutePath());
    this->contentView->detailView->tabView->setCurrentIndex(1);
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
