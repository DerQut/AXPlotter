#ifndef INFERREDVARIABLESVIEW_H
#define INFERREDVARIABLESVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "toggleviewhost.h"

class ContentView;

class InferredVariablesView : public ToggleViewHost
{
    Q_OBJECT
public:
    explicit InferredVariablesView(ContentView* parent = 0);

    QWidget* scrollWidget;
    QScrollArea* scrollArea;
    QVBoxLayout* scrollVStack;

signals:

public slots:
    void refreshVariables();
    void setDarkTheme();
    void setDefaultTheme();
    void setClassicTheme();

};

#endif // INFERREDVARIABLESVIEW_H
