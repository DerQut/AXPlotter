#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#pragma once

#include <QWidget>
#include <QTabWidget>

class ContentView;
class EditorView;
class GraphsView;
class StackedGraphsView;

class DetailView : public QWidget
{
    Q_OBJECT
public:
    explicit DetailView(ContentView *parent = 0);
    ContentView* contentView;
    EditorView* editorView;
    GraphsView* graphsView;
    QTabWidget* tabView;
    StackedGraphsView* stackedGraphsView;

signals:

public slots:

};

#endif // DETAILVIEW_H
