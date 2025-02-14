#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#pragma once

#include <QWidget>

class ContentView;
class EditorView;

class DetailView : public QWidget
{
    Q_OBJECT
public:
    explicit DetailView(ContentView *parent = 0);
    ContentView* contentView;
    EditorView* editorView;

signals:

public slots:

};

#endif // DETAILVIEW_H
