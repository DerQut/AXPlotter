#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtGlobal>
#include <QDebug>

#include "convertaxmtopy.h"

QString convertAXMtoPy(QString axmLine) {

    // Value to be returned
    QString result = QString();

    // Uncomment allowed python code
    QRegularExpression regexPythonCall ("^#\\$(.*)\\#;$");
    QRegularExpressionMatch matchPythonCall = regexPythonCall.match(axmLine);
    if (matchPythonCall.hasMatch()) {
        result += "\n" + matchPythonCall.captured(1);
        return result;
    }

    // Regex to find "variable" calls
    QRegularExpression regexVariable ("\\s*[vV][aA][rR][iI][aA][bB][lL][eE]\\s+(.+)\\s*=\\s*(.+)\\s*;");
    QRegularExpressionMatch matchVariable = regexVariable.match(axmLine);
    if (matchVariable.hasMatch()) {
        result += "\n" + matchVariable.captured(1).trimmed() + " = AXVariable(\"" +matchVariable.captured(1).trimmed()+ "\")";
        result += "\n" + matchVariable.captured(1).trimmed() + ".setCurrentValue(" + matchVariable.captured(2) + ")\n";
        return result;
    }

    // Regex to find defult timesteps: 1 "message" a=8, b=10;
    QRegularExpression regexTimeStepDefault ("^\\s*(\\d+(?:[\\:]\\d+)*)\\s+(.*);");
    QRegularExpressionMatch matchTimeStepDefault = regexTimeStepDefault.match(axmLine);

    QRegularExpression regexTimeStepBracketed ("^\\s*\\[(.*)\\]\\s+(.*);");
    QRegularExpressionMatch matchTimeStepBracketed = regexTimeStepBracketed.match(axmLine);

    QRegularExpression regexDirectAssignment ("(.+)\\s*=\\s*(.+)");
    QRegularExpression regexToAssignmentDefault ("^(.+)\\s+[tT][oO]\\s+(.*)$");
    QRegularExpression regexToAssignmentIn ("^(.+)\\s+[tT][oO]\\s+(.*)\\s+[iI][nN](.*)$");

    QRegularExpression regexLoop ("^[lL][oO][oO][pP]\\s+(.*)\\s*{$");
    QRegularExpressionMatch matchLoop = regexLoop.match(axmLine.trimmed());

    QRegularExpression regexWithAssignment ("(.*)\\s+[wW][iI][tT][hH]\\s+(.*)");
    QRegularExpression regexWithInAssignment ("(.*)\\s+[wW][iI][tT][hH]\\s+(.*)\\s+[iI][nN]\\s+(.*)");

    QRegularExpression regexFollow ("(.*)\\s+follow\\s+(.*)");

    QRegularExpression regexUntil ("\\buntil[\\ ]*(.*)");

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
        return result;
    }

    if (axmLine.trimmed() == "}") {
        result += "\n#AXLOOPEND$";
        return result;
    }

    // If is not a loop or a variable declaration, it has to be a process step:

    QString timestepLength = "0";
    if (matchTimeStepDefault.hasMatch()) {
        timestepLength = matchTimeStepDefault.captured(1);

        QStringList timestepList = timestepLength.split(":");

        // Determine time step length
        int calculatedTimestep = 0;
        int calculatedPart = 0;
        bool convertionSuccesful;

        for (int i=0; i<timestepList.count(); i++) {
            // iterate backwards (first seconds, then minutes, then hours, then a unit that does not exist but let's pretend it does)
            calculatedPart = timestepList.at(timestepList.count()-i-1).toInt(&convertionSuccesful);
            if (convertionSuccesful) {
                for (int j=0; j<i; j++) {
                    // Convert minutes and hours into seconds
                    calculatedPart = calculatedPart * 60;
                }
                calculatedTimestep += calculatedPart;
            }
        }
        timestepLength = QString::number(calculatedTimestep);

    } else if (matchTimeStepBracketed.hasMatch()) {
        timestepLength = matchTimeStepBracketed.captured(1);
    }

    // if neither regex match, then no timestep is given (conditional step, "until"), so the timestep remains at 0

    result += "\nAX_STEP_LENGTH = " + timestepLength;

    QStringList arguments;

    if (matchTimeStepDefault.hasMatch()) {
        arguments = matchTimeStepDefault.captured(2).split(",");
    } else if (matchTimeStepBracketed.hasMatch()) {
        arguments = matchTimeStepBracketed.captured(2).split(",");
    } else {
        axmLine.remove(QRegularExpression(";"));
        arguments = axmLine.split(",");
    }

    QString preResult = QString();
    QString conditions = QString();

    // List containing all time step lengths to prepare a single while loop
    QStringList timesteps;
    timesteps << timestepLength;

    for (int i = 0; i < arguments.size(); i++) {
        QString argument = arguments[i].trimmed();

        // Check if the argument matches the "until" check
        QRegularExpressionMatch matchUntil = regexUntil.match(argument);
        if (matchUntil.hasMatch()) {
            QString condition = matchUntil.captured(1).trimmed();

            conditions += "\nif not (" +condition+ "):";
            conditions += "\n    sys.stderr.write('Warning: condition \"" + condition+ "\" may never be met. Continuing.')";
            continue;
        }

        // Check if the given positional argument creates a follower
        QRegularExpressionMatch matchFollow = regexFollow.match(argument);
        if (matchFollow.hasMatch()) {

            result += "\n" +matchFollow.captured(1)+ ".setCurrentValue(" +matchFollow.captured(1)+ ".currentValue)";

            preResult += "\n    if x <= AX_STEP_LENGTH:";
            preResult += "\n        " +matchFollow.captured(1)+ ".followLambda = lambda: " + matchFollow.captured(2);
            preResult += "\n        " +matchFollow.captured(1)+ + ".calculateFollower()";
            continue;
        }

        // Check if the given positional argument matches a direct assignment (a = 8)
        QRegularExpressionMatch matchDirectAssignment = regexDirectAssignment.match(argument);
        if (matchDirectAssignment.hasMatch()) {
            QString variableName = matchDirectAssignment.captured(1).trimmed();
            QString variableEndGoal = matchDirectAssignment.captured(2).trimmed();

            // Replace "default" keyword with a proper variable
            variableEndGoal.replace(QRegularExpression("\\bdefault\\b"), " " +variableName+ ".physDef ");

            result += "\n" +variableName+ ".endValue = " +variableEndGoal;
            result += "\n" +variableName+ ".setCurrentValue(" +variableName+ ".currentValue)";

            preResult += "\n    if x <= AX_STEP_LENGTH:";
            preResult += "\n        " + variableName + ".setCurrentValue(" + variableEndGoal + ")";
            continue;
        }

        // Check if the given positional atgument matches a "to ... in" assignment (a to 8 in 15)
        QRegularExpressionMatch matchToAssignmentIn = regexToAssignmentIn.match(argument);

        // Check if the given positional argument matches a "to" assignment (a to 8)
        QRegularExpressionMatch matchToAssignmentDefault = regexToAssignmentDefault.match(argument);

        if (matchToAssignmentIn.hasMatch() || matchToAssignmentDefault.hasMatch()) {

            QString variableName;
            QString variableEndGoal;
            QString variableEndTime;

            if (matchToAssignmentIn.hasMatch()) {
                variableName = matchToAssignmentIn.captured(1).trimmed();
                variableEndGoal = matchToAssignmentIn.captured(2).trimmed();
                variableEndTime = matchToAssignmentIn.captured(3).trimmed();
                timesteps << variableEndTime;
            } else {
                variableName = matchToAssignmentDefault.captured(1).trimmed();
                variableEndGoal = matchToAssignmentDefault.captured(2).trimmed();
                variableEndTime = "AX_STEP_LENGTH";
            }

            // Replace "default" keyword in variableEndGoal with a proper variable
            variableEndGoal.replace(QRegularExpression("\\bdefault\\b"), " " +variableName+ ".physDef ");

            // Replace "default" keyword in variableEndTime with a proper variable (should not ever match)
            variableEndTime.replace(QRegularExpression("\\bdefault\\b"), " " +variableName+ ".physDef ");

            result += "\n" +variableName+ ".endValue = " +variableEndGoal;
            result += "\n" +variableName+ ".setCurrentValue(" +variableName+ ".currentValue)";

            preResult += "\n    if " +variableEndTime+ " == 0:";
            preResult += "\n        sys.stderr.write(\"Error: no timestep given for linear ramp. Affected variable: " +variableName+ ". Exiting.\")";
            preResult += "\n        sys.exit()";

            result += "\n" +variableName+ "_AXCOPY = " +variableName+ ".currentValue";

            preResult += "\n    if x <= " +variableEndTime+ ":";
            preResult += "\n        " +variableName+ ".setCurrentValue(" +variableName+ "_AXCOPY + x * (" +variableEndGoal+ " - " +variableName+ "_AXCOPY) / " +variableEndTime + ")";
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
                timesteps << variableEndTime;
            } else {
                variableName = matchWithAssignment.captured(1).trimmed();
                equation = matchWithAssignment.captured(2).trimmed();
                variableEndTime = "AX_STEP_LENGTH";
            }

            result += "\n" +variableName+ ".setCurrentValue(" +variableName+ ".currentValue)";

            preResult += "\n    if x <= " +variableEndTime+ ":";
            preResult += "\n        " +variableName+ ".setCurrentValue(" +equation+ ")";
            continue;
        }

    }
    result += "\nAXTIMESTEPS = [" +timesteps.join(',')+ "]";
    result += "\nx=0";
    result += "\nwhile x <= max(AXTIMESTEPS):";
    result += preResult;
    result += "\n    AX_GLOBAL_TIMESTEP = AX_GLOBAL_TIMESTEP + 1";
    result += "\n    x=x+1\n";

    // Force AX_GLOBAL_TIMESTEP to be increased by the specified timestep for the whole step
    result += "\nAX_GLOBAL_TIMESTEP = AX_GLOBAL_TIMESTEP + AXTIMESTEPS[0] - max(AXTIMESTEPS)\n";

    result += conditions;

    //result += "\n\nAX_GLOBAL_TIMESTEP = AX_GLOBAL_TIMESTEP + AX_STEP_LENGTH";
    return result;
}
