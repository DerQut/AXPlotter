#ifndef GRAPHSVIEW_H
#define GRAPHSVIEW_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

#include "qcustomplot/qcustomplot.h"

class ContentView;

class GraphsView : public QWidget
{
    Q_OBJECT
public:
    explicit GraphsView(ContentView *parent = 0);
    ContentView* contentView;

    QVBoxLayout* scrollVStack;
    QVector<QCustomPlot*> plots;

    QSlider* xMinSlider;
    QSlider* xMaxSlider;

    QLabel* xMinLabel;
    QLabel* xMaxLabel;

signals:

public slots:
    void updatePlots(QString directoryName);
    void deletePlots();
    void setPlotsXRange(int xMin, int xMax);

    void updatePlotsRange();
    void xMinSliderCall();
    void xMaxSliderCall();

};

void clearLayout(QLayout* layout);

#endif // GRAPHSVIEW_H
