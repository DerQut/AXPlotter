#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "convertaxmtopy.h"

QString convertAXMtoPy(QString axmLine) {

    // Value to be returned
    QString result = QString();

    // Regex to find "variable" calls
    QRegularExpression regexVariable ("\\s*[vV][aA][rR][iI][aA][bB][lL][eE]\\s+(.+)\\s*=\\s*(.+)\\s*;");
    QRegularExpressionMatch matchVariable = regexVariable.match(axmLine);

    if (matchVariable.hasMatch()) {
        result = (matchVariable.captured(1).trimmed() + " = " +  matchVariable.captured(2));
        return result;
    }

    return result;
}
