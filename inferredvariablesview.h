#ifndef INFERREDVARIABLESVIEW_H
#define INFERREDVARIABLESVIEW_H

#include <QWidget>

class ContentView;

class InferredVariablesView : public QWidget
{
    Q_OBJECT
public:
    explicit InferredVariablesView(ContentView* parent = 0);
    ContentView* contentView;

signals:

public slots:

};

#endif // INFERREDVARIABLESVIEW_H
