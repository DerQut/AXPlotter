#ifndef INFERREDVARIABLESVIEW_H
#define INFERREDVARIABLESVIEW_H

#include <QWidget>
#include <QPushButton>
#include "toggleviewhost.h"

class ContentView;

class InferredVariablesView : public ToggleViewHost
{
    Q_OBJECT
public:
    explicit InferredVariablesView(ContentView* parent = 0);
    void refreshVariables();

signals:

public slots:

};

#endif // INFERREDVARIABLESVIEW_H
