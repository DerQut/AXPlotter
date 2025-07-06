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

    // Regex to find defult timesteps: 1 "message" a=8, b=10;
    QRegularExpression regexTimeStepDefault ("(\\d+)\\s+[\"](.*)[\"](.*);");
    QRegularExpressionMatch matchTimeStepDefault = regexTimeStepDefault.match(axmLine);

    QRegularExpression regexDirectAssignment ("(.+)\\s*=\\s*(.+)");
    QRegularExpression regexToAssignmentDefault ("^(.+)\\s+[tT][oO]\\s+([^\\s]+)$");

    if (matchTimeStepDefault.hasMatch()) {
        result = QString();
        result += "\n#" + matchTimeStepDefault.captured(2);
        result += "\ni=0";
        result += "\nwhile i <= " + matchTimeStepDefault.captured(1) + ":";

        QStringList arguments = matchTimeStepDefault.captured(3).split(",");
        for (int i = 0; i < arguments.size(); i++) {
            QString argument = arguments[i].trimmed();

            // Check if the given positional argument matches a direct assignment (a = 8)
            QRegularExpressionMatch matchDirectAssignment = regexDirectAssignment.match(argument);
            if (matchDirectAssignment.hasMatch()) {
                result += "\n    " + argument.trimmed();
                continue;
            }

            // Check if the given positional argument matches a "to" assignment (a to 8)
            QRegularExpressionMatch matchToAssignmentDefault = regexToAssignmentDefault.match(argument);
            if (matchToAssignmentDefault.hasMatch()) {
                QString variableName = matchToAssignmentDefault.captured(1);
                QString variableEndGoal = matchToAssignmentDefault.captured(2);
                result += "\n    if i == 0:";
                result += "\n        " + variableName + "_AXCOPY = " + variableName;
                result += "\n    " + variableName + " = " + variableName + "_AXCOPY + i * (" + variableEndGoal + " - " + variableName + "_AXCOPY) / " +matchTimeStepDefault.captured(1);
                continue;
            }
        }

        result += "\n    i = i + 1";
    }

    return result;
}
