#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtGlobal>
#include <QDebug>

#include "convertaxmtopy.h"

QString convertAXMtoPy(QString axmLine) {

    // Value to be returned
    QString result = QString();

    // Regex to find "variable" calls
    QRegularExpression regexVariable ("\\s*[vV][aA][rR][iI][aA][bB][lL][eE]\\s+(.+)\\s*=\\s*(.+)\\s*;");
    QRegularExpressionMatch matchVariable = regexVariable.match(axmLine);

    if (matchVariable.hasMatch()) {
        result = (matchVariable.captured(1).trimmed() + " = " +  matchVariable.captured(2));
        result += "\nfile = open(\"" +matchVariable.captured(1).trimmed()+ ".csv\", \"a+\")";
        result += "\nfile.write('0,' + str(" +matchVariable.captured(1).trimmed()+ ") + '\\n')";
        result += "\nfile.close()\n";

        return result;
    }

    // Regex to find defult timesteps: 1 "message" a=8, b=10;
    QRegularExpression regexTimeStepDefault ("(\\S+)\\s+(.*);");
    QRegularExpressionMatch matchTimeStepDefault = regexTimeStepDefault.match(axmLine);

    QRegularExpression regexDirectAssignment ("(.+)\\s*=\\s*(.+)");
    QRegularExpression regexToAssignmentDefault ("^(.+)\\s+[tT][oO]\\s+(.*)$");
    QRegularExpression regexToAssignmentIn ("^(.+)\\s+[tT][oO]\\s+(.*)\\s+[iI][nN](.*)$");

    QRegularExpression regexLoop ("^[lL][oO][oO][pP]\\s+(.*)\\s*{$");
    QRegularExpressionMatch matchLoop = regexLoop.match(axmLine.trimmed());

    QRegularExpression regexWithAssignment ("(.*)\\s+[wW][iI][tT][hH]\\s+(.*)");
    QRegularExpression regexWithInAssignment ("(.*)\\s+[wW][iI][tT][hH]\\s+(.*)\\s+[iI][nN]\\s+(.*)");

    if (matchLoop.hasMatch()) {
        // Generate a random string for loop iterator (allows for nested loops)
        QString chars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
        QString randomString = QString();
        for (int i = 0; i < 16; i++) {
            randomString += chars.at(qrand() % chars.size());
        }
        result += "\n\n" +randomString+ "_AXLOOPITERATOR = 0";
        result += "\nwhile " +randomString+ "_AXLOOPITERATOR < " +matchLoop.captured(1)+ ":";
        result += "\n    " +randomString+ "_AXLOOPITERATOR = " +randomString+ "_AXLOOPITERATOR + 1 #AXLOOPSTART$";
    }

    if (axmLine.trimmed() == "}") {
        result += "\n#AXLOOPEND$";
    }

    if (matchTimeStepDefault.hasMatch()) {

        QString timestepLength = matchTimeStepDefault.captured(1);
        timestepLength.remove("]");
        timestepLength.remove("[");

        result = QString();
        result += "\nAX_STEP_LENGTH = " + timestepLength;

        QStringList arguments = matchTimeStepDefault.captured(2).split(",");
        for (int i = 0; i < arguments.size(); i++) {
            QString argument = arguments[i].trimmed();

            // Check if the given positional argument matches a direct assignment (a = 8)
            QRegularExpressionMatch matchDirectAssignment = regexDirectAssignment.match(argument);
            if (matchDirectAssignment.hasMatch()) {
                QString variableName = matchDirectAssignment.captured(1).trimmed();
                QString variableEndGoal = matchDirectAssignment.captured(2).trimmed();

                // Replace "default" keyword with a proper variable
                variableEndGoal.replace(QRegularExpression("\\b[dD][eE][fF][aA][uU][lL][tT]"), " " +variableName+ "_AXDEFAULT ");

                result += "\n\ntry:";
                result += "\n    " +variableName+ "_AXDEFAULT = " +variableName+ "_AXDEFAULT";
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXDEFAULT = 0";

                result += "\nfile = open(\"" + variableName.trimmed() + ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i <= AX_STEP_LENGTH:";
                result += "\n    " + variableName + " = " + variableEndGoal;
                result += "\n    file.write(str(AX_GLOBAL_TIMESTEP+i) + ',' + str(" + variableName+ ") + '\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }

            // Check if the given positional atgument matches a "to ... in" assignment (a to 8 in 15)
            QRegularExpressionMatch matchToAssignmentIn = regexToAssignmentIn.match(argument);
            if (matchToAssignmentIn.hasMatch()) {
                QString variableName = matchToAssignmentIn.captured(1).trimmed();
                QString variableEndGoal = matchToAssignmentIn.captured(2).trimmed();
                QString variableEndTime = matchToAssignmentIn.captured(3).trimmed();

                // Replace "default" keyword with a proper variable
                variableEndGoal.replace(QRegularExpression("\\b[dD][eE][fF][aA][uU][lL][tT]"), " " +variableName+ "_AXDEFAULT ");

                // Replace "default" keyword with a proper variable (should not ever match)
                variableEndTime.replace(QRegularExpression("\\b[dD][eE][fF][aA][uU][lL][tT]"), " " +variableName+ "_AXDEFAULT ");

                result += "\n\ntry:";
                result += "\n    " +variableName+ "_AXDEFAULT = " +variableName+ "_AXDEFAULT";
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXDEFAULT = 0";

                result += "\ntry:";
                result += "\n    " +variableName+ "_AXCOPY = " +variableName;
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXCOPY = " +variableName+ "_AXDEFAULT";
                result += "\n    " +variableName+ " = " +variableName+ "_AXDEFAULT";
                result += "\nfile = open(\"" +variableName.trimmed()+ ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i <= " +variableEndTime+ ":";
                result += "\n    " +variableName+ " = " +variableName+ "_AXCOPY + i * (" +variableEndGoal+ " - " +variableName+ "_AXCOPY) / " +variableEndTime;
                result += "\n    file.write(str(AX_GLOBAL_TIMESTEP+i) + ',' + str(" +variableName+ ") + '\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }

            // Check if the given positional argument matches a "to" assignment (a to 8)
            QRegularExpressionMatch matchToAssignmentDefault = regexToAssignmentDefault.match(argument);

            if (matchToAssignmentDefault.hasMatch()) {
                QString variableName = matchToAssignmentDefault.captured(1).trimmed();
                QString variableEndGoal = matchToAssignmentDefault.captured(2);

                // Replace "default" keyword with a proper variable
                variableEndGoal.replace(QRegularExpression("\\b[dD][eE][fF][aA][uU][lL][tT]"), " " +variableName+ "_AXDEFAULT ");
                variableEndGoal = variableEndGoal.trimmed();

                result += "\n\ntry:";
                result += "\n    " +variableName+ "_AXDEFAULT = " +variableName+ "_AXDEFAULT";
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXDEFAULT = 0";

                result += "\ntry:";
                result += "\n    " +variableName+ "_AXCOPY = " +variableName;
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXCOPY = " +variableName+ "_AXDEFAULT";
                result += "\n    " +variableName+ " = " +variableName+ "_AXDEFAULT";
                result += "\nfile = open(\"" +variableName.trimmed()+ ".csv\", \"a+\")";

                result += "\ni = 0";
                result += "\nwhile i <= AX_STEP_LENGTH:";
                result += "\n    " +variableName+ " = " +variableName+ "_AXCOPY + i * (" +variableEndGoal+ " - " +variableName+ "_AXCOPY) / AX_STEP_LENGTH";
                result += "\n    file.write(str(AX_GLOBAL_TIMESTEP+i) + ',' + str(" +variableName+ ") + '\\n')";
                result += "\n    i = i + 1";
                result += "\nfile.close()";
                continue;
            }

            // Check if the given positional argument matches a "with ... in" assignment (a with x^3 in 15)
            QRegularExpressionMatch matchWithInAssignment = regexWithInAssignment.match(argument);

            // Check if the given positional argument matches a "with" assignment (a with sin(x))
            QRegularExpressionMatch matchWithAssignment = regexWithAssignment.match(argument);

            if (matchWithInAssignment.hasMatch() || matchWithAssignment.hasMatch()) {

                QString variableName;
                QString equation;
                QString variableEndTime;

                if (matchWithInAssignment.hasMatch()) {
                    variableName = matchWithInAssignment.captured(1).trimmed();
                    equation = matchWithInAssignment.captured(2).trimmed();
                    variableEndTime = matchWithInAssignment.captured(3);
                } else {
                    variableName = matchWithAssignment.captured(1).trimmed();
                    equation = matchWithAssignment.captured(2).trimmed();
                    variableEndTime = "AX_STEP_LENGTH";
                }

                result += "\n\ntry:";
                result += "\n    " +variableName+ "_AXDEFAULT = " +variableName+ "_AXDEFAULT";
                result += "\nexcept NameError:";
                result += "\n    " +variableName+ "_AXDEFAULT = 0";

                result += "\nfile = open(\"" +variableName.trimmed()+ ".csv\", \"a+\")";

                result += "\nx = 0";
                result += "\nwhile x <= AX_STEP_LENGTH:";
                result += "\n    " +variableName+ " = " +equation;
                result += "\n    file.write(str(AX_GLOBAL_TIMESTEP+x) + ',' + str(" +variableName+ ") + '\\n')";
                result += "\n    x = x + 1";
                result += "\nfile.close()";

                continue;
            }

        }

        result += "\n\nAX_GLOBAL_TIMESTEP = AX_GLOBAL_TIMESTEP + AX_STEP_LENGTH";
    }

    return result;
}
