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
        QStringLiteral("\\bvariable\\b"),
        QStringLiteral("\\bparameter\\b"),
        QStringLiteral("\\bdeviation\\b"),
        QStringLiteral("\\bdefault\\b"),
        QStringLiteral("\\bwarning\\b"),
        QStringLiteral("\\balarm\\b"),
        QStringLiteral("\\buntil\\b"),
        QStringLiteral("\\bin\\b"),
        QStringLiteral("\\bto\\b"),
        QStringLiteral("\\bwith\\b"),
        QStringLiteral("\\bfollow\\b"),
        QStringLiteral("\\bbegin\\b"),
        QStringLiteral("\\bend\\b"),
        QStringLiteral("\\bstat\\b"),
        QStringLiteral("\\bloop\\b"),
        QStringLiteral("\\bopen\\b"),
        QStringLiteral("\\bclose\\b"),
        QStringLiteral("\\bon\\b"),
        QStringLiteral("\\boff\\b"),
        // Python keywords
        QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bexcept\\b"),
        QStringLiteral("\\bif\\b"),
        QStringLiteral("\\belse\\b"),
        QStringLiteral("\\belif\\b"),
        QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bclass\\b")
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
