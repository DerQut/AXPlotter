#ifndef LINENUMBERSVIEW_H
#define LINENUMBERSVIEW_H

#include <QWidget>
#include <QTextEdit>

class EditorView;


class LineNumbersView : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumbersView(EditorView *parent = 0);
    QTextEdit* lineDisplay;

signals:

public slots:

};

#endif // LINENUMBERSVIEW_H
