#ifndef STACKEDGRAPHSVIEW_H
#define STACKEDGRAPHSVIEW_H

#include <QWidget>

class ContentView;

class StackedGraphsView : public QWidget
{
    Q_OBJECT
public:
    explicit StackedGraphsView(ContentView* parent = 0);
    ContentView* contentView;

signals:

public slots:

};

#endif // STACKEDGRAPHSVIEW_H
