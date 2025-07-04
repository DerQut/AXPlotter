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

    // Try to open the file
    if (!(fileHeader.open(QIODevice::ReadOnly | QIODevice::Text))) {
        return QString();
    }

    // Variable to store the read text
    QString result = QString();

    // Regular expression to check for "read" call
    QRegularExpression regexRead("^read\\s+(.+);$");

    //Regular expression to check if a file extention is given
    QRegularExpression regexExtention("(\\w+)[.](\\w+)");

    // Create a text stream for reading the script file
    QTextStream in (&fileHeader);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        QRegularExpressionMatch matchRead = regexRead.match(line);
        if (matchRead.hasMatch()) {

            // Inferred file name
            QString newFile;

            // Check if the captured file has an extention
            QRegularExpressionMatch matchExtention = regexExtention.match(matchRead.captured(1));
            if (matchExtention.hasMatch()) {
                newFile = matchRead.captured(1) + "." + matchExtention.captured(2);
            } else {
                newFile = matchRead.captured(1) + ".epi";
            }
            line = recursiveReplaceRead(QFileInfo(fileName).absolutePath() + QDir::separator() + newFile);
        }
        result += line.trimmed() + "\n";
    }

    // Close the file
    fileHeader.close();

    return result;
}
