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
    QRegularExpression regexTimeStepDefault ("(.+)\\s+[\"](.*)[\"](.*);");
    QRegularExpressionMatch matchTimeStepDefault = regexTimeStepDefault.match(axmLine);

    QRegularExpression regexDirectAssignment ("(.+)\\s*=\\s*(.+)");
    QRegularExpression regexToAssignmentDefault ("^(.+)\\s+[tT][oO]\\s+([^\\s]+)$");
    QRegularExpression regexToAssignmentUntil ("^(.+)\\s+[tT][oO]\\s+([^\\s]+)$");

    if (matchTimeStepDefault.hasMatch()) {

        QString timestepLength = matchTimeStepDefault.captured(1);
        timestepLength.remove("]");
        timestepLength.remove("[");

        result = QString();
        result += "\n\n#" + matchTimeStepDefault.captured(2);
        result += "\nAX_STEP_LENGTH = " + timestepLength;

        QStringList arguments = matchTimeStepDefault.captured(3).split(",");
        for (int i = 0; i < arguments.size(); i++) {
            QString argument = arguments[i].trimmed();

            // Check if the given positional argument matches a direct assignment (a = 8)
            QRegularExpressionMatch matchDirectAssignment = regexDirectAssignment.match(argument);
            if (matchDirectAssignment.hasMatch()) {
                QString variableName = matchDirectAssignment.captured(1);
                QString variableEndGoal = matchDirectAssignment.captured(2);

                result += "\nfile = open(\"" + variableName.trimmed() + ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i < AX_STEP_LENGTH:";
                result += "\n    " + variableName + " = " + variableEndGoal;
                result += "\n    file.write(f'{AX_GLOBAL_TIMESTEP+i},{" + variableName+ "}\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }

            // Check if the given positional argument matches a "to" assignment (a to 8)
            QRegularExpressionMatch matchToAssignmentDefault = regexToAssignmentDefault.match(argument);
            if (matchToAssignmentDefault.hasMatch()) {
                QString variableName = matchToAssignmentDefault.captured(1);
                QString variableEndGoal = matchToAssignmentDefault.captured(2);

                result += "\n\ntry:";
                result += "\n    " + variableName + "_AXCOPY = " + variableName;
                result += "\nexcept NameError:";
                result += "\n    try:";
                result += "\n        " + variableName + "_AXCOPY = " + variableName + "_AXDEFAULT";
                result += "\n        " + variableName + " = " + variableName + "_AXDEFAULT";
                result += "\n    except NameError:";
                result += "\n        " + variableName + " = 0";
                result += "\n        " + variableName + "_AXCOPY = 0";
                result += "\n\nfile = open(\"" + variableName.trimmed() + ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i < AX_STEP_LENGTH:";
                result += "\n    " + variableName + " = " + variableName + "_AXCOPY + (i+1) * (" + variableEndGoal + " - " + variableName + "_AXCOPY) / AX_STEP_LENGTH";
                result += "\n    file.write(f'{AX_GLOBAL_TIMESTEP+i},{" + variableName+ "}\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }

            // Check if the given positional atgument matches a "to ... in" assignment (a to 8 in 15)
            QRegularExpressionMatch matchToAssignmentUntil = regexToAssignmentUntil.match(argument);
            if (matchToAssignmentUntil.hasMatch()) {
                QString variableName = matchToAssignmentUntil.captured(1);
                QString variableEndGoal = matchToAssignmentUntil.captured(2);
                QString variableEndTime = matchToAssignmentUntil.captured(3);

                result += "\n\ntry:";
                result += "\n    " + variableName + "_AXCOPY = " + variableName;
                result += "\nexcept NameError:";
                result += "\n    try:";
                result += "\n        " + variableName + "_AXCOPY = " + variableName + "_AXDEFAULT";
                result += "\n        " + variableName + " = " + variableName + "_AXDEFAULT";
                result += "\n    except NameError:";
                result += "\n        " + variableName + " = 0";
                result += "\n        " + variableName + "_AXCOPY = 0";
                result += "\n\nfile = open(\"" + variableName.trimmed() + ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i < AX_STEP_LENGTH:";
                result += "\n    " + variableName + " = " + variableName + "_AXCOPY + (i+1) * (" + variableEndGoal + " - " + variableName + "_AXCOPY) / " + variableEndTime;
                result += "\n    file.write(f'{AX_GLOBAL_TIMESTEP+i},{" + variableName+ "}\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }
        }

        result += "\n\nAX_GLOBAL_TIMESTEP = AX_GLOBAL_TIMESTEP + AX_STEP_LENGTH";
    }

    return result;
}
