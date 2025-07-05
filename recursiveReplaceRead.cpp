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
        return QString();
    }

    // Variable to store the read text
    QString result = QString();

    // Regular expression to check for "read" call
    QRegularExpression regexRead("^read\\s+(.+);$");

    //Regular expression to check if a file extention is given
    // At least one letter, dot, at least one letter
    QRegularExpression regexExtention("(\\w+)[.](\\w+)");

    // Create a text stream for reading the script file
    QTextStream in (&fileHeader);
    while (!in.atEnd()) {
        QString line = in.readLine().simplified();

        QRegularExpressionMatch matchRead = regexRead.match(line);
        if (matchRead.hasMatch()) {

            // Inferred file name
            QString newFile = QFileInfo(fileName).absolutePath() + QDir::separator();

            // Check if the captured file has an extention
            QRegularExpressionMatch matchExtention = regexExtention.match(matchRead.captured(1));

            if (matchExtention.hasMatch()) {
                // if the file has an extention, use it
                newFile += matchRead.captured(1) + "." + matchExtention.captured(2);
            } else {
                // if the file has no extention, use the default one (.epi)
                newFile += matchRead.captured(1) + ".epi";
            }

            // Check if the file is not trying to read itself
            if (newFile != filename) {
                line = recursiveReplaceRead(newFile);
            } else {
                line = QString();
            }
        }
        result += line.trimmed() + "\n";
    }

    // Close the file
    fileHeader.close();

    return result;
}
