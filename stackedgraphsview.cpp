#include <QVBoxLayout>
#include <QColor>
#include <QTime>

#include "stackedgraphsview.h"
#include "axdataseries.h"
#include "contentview.h"
#include "contentview.h"

#include "qcustomplot/qcustomplot.h"

StackedGraphsView::StackedGraphsView(ContentView* parent) :
    QWidget(parent)
{
    qsrand(uint(QTime::currentTime().msec()));

    this->contentView = parent;
    QVBoxLayout* mainVStack = new QVBoxLayout();
    mainVStack->setContentsMargins(0,0,0,0);

    this->plot = new QCustomPlot();

    plot->legend->setVisible(true);
    plot->legend->setBrush(QColor(255, 255, 255, 200));
    plot->legend->setBorderPen(QPen(Qt::black));

    mainVStack->addWidget(plot);

    this->setLayout(mainVStack);

    this->replot();
}


void StackedGraphsView::replot() {

    plot->clearPlottables();

    for (std::int64_t i = 0; i < this->contentView->inferredVariables.count(); i++) {

        QCPCurve* curve = new QCPCurve(plot->xAxis, plot->yAxis);
        curve->setPen(QPen(this->contentView->inferredVariables[i].color, 2));

        curve->addData(this->contentView->inferredVariables[i].xData, this->contentView->inferredVariables[i].multipliedYData);
        curve->setName(this->contentView->inferredVariables[i].variableName);
    }

    plot->rescaleAxes();
    plot->replot();

}
