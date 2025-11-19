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

    QSlider* xMinSlider;
    QSlider* xMaxSlider;

    QLabel* xMinLabel;
    QLabel* xMaxLabel;

signals:

public slots:
    void replot();

    void setPlotsXRange(int xMin, int xMax);

    void updatePlotsRange();
    void xMinSliderCall();
    void xMaxSliderCall();

};

#endif // STACKEDGRAPHSVIEW_H
