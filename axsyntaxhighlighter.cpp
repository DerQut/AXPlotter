#include <QSyntaxHighlighter>
#include <QString>

#include "axsyntaxhighlighter.h"

AXSyntaxHighlighter::AXSyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Setting up regex recognition for language keywords
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    const QString keywordPatterns[] = {
        QStringLiteral("\\b[vV][aA][rR][iI][aA][bB][lL][eE]\\b"),
        QStringLiteral("\\b[pP][aA][rR][aA][mM][eE][tT][eE][rR]\\b"),
        QStringLiteral("\\b[dD][eE][vV][iI][aA][tT][iI][oO][nN]\\b"),
        QStringLiteral("\\b[dD][eE][fF][aA][uU][lL][tT]\\b"),
        QStringLiteral("\\b[wW][aA][rR][nN][iI][nN][gG]\\b"),
        QStringLiteral("\\b[aA][lL][aA][rR][mM]\\b"),
        QStringLiteral("\\b[uU][nN][tT][iI][lL]\\b"),
        QStringLiteral("\\b[iI][nN]\\b"),
        QStringLiteral("\\b[tT][oO]\\b"),
        QStringLiteral("\\b[wW][iI][tT][hH]\\b"),
        QStringLiteral("\\b[fF][oO][lL][lL][oO][wW]\\b"),
        QStringLiteral("\\b[bB][eE][gG][iI][nN]\\b"),
        QStringLiteral("\\b[eE][nN][dD]\\b"),
        QStringLiteral("\\b[sS][tT][aA][tT]\\b"),
        QStringLiteral("\\b[lL][oO][oO][pP]\\b"),
        // Python keywords
        QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bexcept\\b"),
        QStringLiteral("\\bwhile\\b")
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


    // Setting up regex recognition for macro definitions. Example:
    //
    // MacroName { ... }
    //
    macroFormat.setForeground(Qt::darkYellow);
    macroFormat.setFontWeight(QFont::Bold);

    const QString macroPatterns[] = {
        QStringLiteral("\\w+\\s*{"),
        QStringLiteral("}")
    };

    for (const QString &pattern : macroPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = macroFormat;
        highlightingRules.append(rule);
    }


    // Setting up regex recognition for wait calls. Example:
    //
    // 10
    //
    // [Waiting_Period ]
    //
    waitFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\A\\s*\\d+"));
    rule.format = waitFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("[[]\\s*\\w*\\s*[]]"));
    highlightingRules.append(rule);


    // Setting up regex recognition for print calls. Example:
    //
    // "Setting the temperature to 150 degrees"
    //
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Setting up regex recognition for code comments. Example:
    //
    // # This is a comment
    //
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Setting up regex recognition for read calls. Example:
    //
    // read Macros.txt
    //
    readFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b[rR][eE][aA][dD]\\s*\\w*");
    rule.format = readFormat;
    highlightingRules.append(rule);

}


void AXSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
