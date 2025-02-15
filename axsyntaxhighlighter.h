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

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // AXSYNTAXHIGHLIGHTER_H
