#ifndef AXSYNTAXHIGHLIGHTER_H
#define AXSYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class AXSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    AXSyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression layerStartExpression;
    QRegularExpression layerEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat macroFormat;
    QTextCharFormat waitFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat layerFormat;
};

#endif // AXSYNTAXHIGHLIGHTER_H
