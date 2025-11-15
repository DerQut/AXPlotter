#ifndef STACKEDGRAPHSVIEW_H
#define STACKEDGRAPHSVIEW_H

#include <QWidget>

#include "qcustomplot/qcustomplot.h"

class ContentView;

class StackedGraphsView : public QWidget
{
    Q_OBJECT
public:
    explicit StackedGraphsView(ContentView* parent = 0);
    ContentView* contentView;
    QCustomPlot* plot;

signals:

public slots:
    void replot();

};

#endif // STACKEDGRAPHSVIEW_H
