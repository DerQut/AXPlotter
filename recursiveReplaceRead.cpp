#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextStream>

#include "recursiveReplaceRead.h"

QString recursiveReplaceRead(QString fileName) {

    // Create a file header
    QFile fileHeader(fileName);

    // Try to open the file, return an empty string upon failure
    if (!(fileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return "$AXR_ERROR_START File " +fileName+ " could not be opened $AXR_ERROR_END";
    }

    // Variable to store the read text
    QString result = QString();

    // Regular expression to check for "read" call
    QRegularExpression regexRead("^\\s*read\\s+(.+);$");

    // Create a text stream for reading the script file
    QTextStream in (&fileHeader);
    while (!in.atEnd()) {
        QString line = in.readLine().simplified();

        QRegularExpressionMatch matchRead = regexRead.match(line);
        if (matchRead.hasMatch()) {

            // Inferred file name
            QString newFile = QFileInfo(fileName).absolutePath() + QDir::separator() + matchRead.captured(1);

            if (!matchRead.captured(1).count(".")) {
                // if the file has no extention, use the default one (.epi)
                newFile += ".epi";
            }

            // Check if the file has dwo or more dots
            if (matchRead.captured(1).count(".") > 1) {
                result += "$AXR_ERROR_START Filename " +newFile+ " Contains more than one '.' character $AXR_ERROR_END";
            }

            // Check if the file is not trying to read itself
            if (newFile != fileName) {
                line = recursiveReplaceRead(newFile);
            } else {
                line = "$AXR_ERROR_START File " +newFile+ "Tried to read itself. $AXR_ERROR_END";
            }
        }
        result += line.trimmed() + "\n";
    }

    // Close the file
    fileHeader.close();

    return result;
}
