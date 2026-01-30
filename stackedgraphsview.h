#ifndef STACKEDGRAPHSVIEW_H
#define STACKEDGRAPHSVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <QColor>

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

    QColorDialog* colorDialog;
    int currentPlotToColor;

signals:

public slots:
    void replot();

    void setPlotsXRange(int xMin, int xMax);

    void updatePlotsRange();
    void xMinSliderCall();
    void xMaxSliderCall();

    void legendClickHandler(QCPLegend* legend, QCPAbstractLegendItem* item, QMouseEvent* event);

    void setColor(QColor color);

};

#endif // STACKEDGRAPHSVIEW_H
