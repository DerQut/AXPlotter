#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QTextEdit>

class DetailView;
class AXSyntaxHighlighter;

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(DetailView *parent = 0);
    DetailView* detailView;
    QTextEdit* textEdit;
    AXSyntaxHighlighter* highlighter;

signals:

public slots:
    void readRecipeFile(QString path);
};

#endif // EDITORVIEW_H
