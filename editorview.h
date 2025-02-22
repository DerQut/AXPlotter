#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QTextEdit>

class DetailView;
class AXSyntaxHighlighter;
class LineNumbersView;

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(DetailView *parent = 0);
    DetailView* detailView;
    QTextEdit* textEdit;
    LineNumbersView* lineNumbersView;
    AXSyntaxHighlighter* highlighter;

signals:

public slots:
    void readRecipeFile(QString path);
};

#endif // EDITORVIEW_H
