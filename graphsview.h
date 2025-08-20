#ifndef GRAPHSVIEW_H
#define GRAPHSVIEW_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>

#include "qcustomplot/qcustomplot.h"

class ContentView;

class GraphsView : public QWidget
{
    Q_OBJECT
public:
    explicit GraphsView(ContentView *parent = 0);
    ContentView* contentView;

    QWidget* scrollWidget;

signals:

public slots:
    void updatePlots(QString directoryName);
    void setPlotsXRange(int xMin, int yMin);

};

#endif // GRAPHSVIEW_H
