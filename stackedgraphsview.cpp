#include <QVBoxLayout>
#include <QColor>
#include <QTime>
#include <QDebug>
#include <QColorDialog>

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

    // Sliders to control the plot X scale

    this->xMinLabel = new QLabel("X range (min): 0", this);
    this->xMinLabel->setFixedWidth(110);
    this->xMinLabel->setAlignment(Qt::AlignLeft);

    this->xMaxLabel = new QLabel("X range (max): 100", this);
    this->xMaxLabel->setFixedWidth(110);
    this->xMaxLabel->setAlignment(Qt::AlignLeft);

    this->xMinSlider = new QSlider(this);
    this->xMinSlider->setRange(0, 100);
    this->xMinSlider->setValue(0);
    this->xMinSlider->setOrientation(Qt::Horizontal);

    this->xMaxSlider = new QSlider(this);
    this->xMaxSlider->setRange(0, 100);
    this->xMaxSlider->setValue(100);
    this->xMaxSlider->setOrientation(Qt::Horizontal);

    QHBoxLayout* xMinHStack = new QHBoxLayout();
    QHBoxLayout* xMaxHStack = new QHBoxLayout();

    xMinHStack->addWidget(this->xMinLabel);
    xMinHStack->addWidget(this->xMinSlider);

    xMaxHStack->addWidget(this->xMaxLabel);
    xMaxHStack->addWidget(this->xMaxSlider);

    mainVStack->addLayout(xMinHStack);
    mainVStack->addLayout(xMaxHStack);

    this->setLayout(mainVStack);

    this->replot();

    this->colorDialog = new QColorDialog(this);
    this->colorDialog->hide();

    this->currentPlotToColor = -1;

    connect(this->xMinSlider, SIGNAL(valueChanged(int)), this, SLOT(xMinSliderCall()));
    connect(this->xMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(xMaxSliderCall()));

    connect(this->plot, SIGNAL(legendClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendClickHandler(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)));

    connect( this->colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(setColor(QColor)) );
}


void StackedGraphsView::replot() {

    plot->clearPlottables();
    this->currentPlotToColor = -1;

    for (std::int64_t i = 0; i < this->contentView->inferredVariables.count(); i++) {

        if (!this->contentView->inferredVariables[i].isVisible) {
            continue;
        }

        QCPCurve* curve = new QCPCurve(plot->xAxis, plot->yAxis);
        curve->setPen(QPen(this->contentView->inferredVariables[i].color, 2));

        curve->addData(this->contentView->inferredVariables[i].xData, this->contentView->inferredVariables[i].multipliedYData);
        curve->setName(this->contentView->inferredVariables[i].variableName);
    }

    this->updatePlotsRange();

}


void StackedGraphsView::setPlotsXRange(int xMin, int xMax) {
    plot->xAxis->setRange(xMin, xMax);

    double yMax = 0;
    double yMin = 0;

    for (int i = 0; i < this->contentView->inferredVariables.count(); i++) {
        AXDataSeries variable = this->contentView->inferredVariables[i];
        if (!variable.isVisible) {
            continue;
        }

        for (int j = 0; j < variable.xData.count(); j++) {
            if (variable.xData[j] < xMin || variable.xData[j] > xMax) {
                continue;
            }

            if (variable.multipliedYData[j] > yMax) {
                yMax = variable.multipliedYData[j];
            }

            if (variable.multipliedYData[j] < yMin) {
                yMin = variable.multipliedYData[j];
            }
        }
    }

    plot->yAxis->setRange(yMin, yMax);

    plot->replot();
}

void StackedGraphsView::updatePlotsRange() {
    qDebug() << "xMin: " << this->xMinSlider->value() << ", xMax: " << this->xMaxSlider->value();
    this->setPlotsXRange(this->xMinSlider->value(), this->xMaxSlider->value());
}

void StackedGraphsView::xMaxSliderCall() {
    this->xMaxLabel->setText("X range (max): " + QString::number(this->xMaxSlider->value()));
    if (xMaxSlider->value() < xMinSlider->value()) {
        xMinSlider->setValue(xMaxSlider->value());
    }
    this->updatePlotsRange();
}

void StackedGraphsView::xMinSliderCall() {
    this->xMinLabel->setText("X range (min): " + QString::number(this->xMinSlider->value()));
    if (xMinSlider->value() > xMaxSlider->value()) {
        xMaxSlider->setValue(xMinSlider->value());
    }
    this->updatePlotsRange();
}


void StackedGraphsView::legendClickHandler(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event) {
    if (!legend) {return;}
    if (!item)   {return;}
    if (!event)  {return;}

    if (!event->button() == Qt::RightButton) {return;}

    auto* plotItem = qobject_cast<QCPPlottableLegendItem*>(item);
    if (!plotItem) {return;}
    QCPAbstractPlottable* plottable = plotItem->plottable();
    qDebug() << plottable->name();

    for (int i = 0; i < this->contentView->inferredVariables.count(); i++) {
        if (this->contentView->inferredVariables[i].variableName == plottable->name()) {

            this->colorDialog->setCurrentColor(this->contentView->inferredVariables[i].color);
            this->colorDialog->setWindowTitle("Select color for \"" +plottable->name()+ "\"");

            this->colorDialog->show();
            this->currentPlotToColor = i;
            return;
        }
    }
}

void StackedGraphsView::setColor(QColor color) {
    if (this->currentPlotToColor == -1) {return;}
    if (this->currentPlotToColor >= contentView->inferredVariables.count()) {return;}

    contentView->inferredVariables[currentPlotToColor].color = color;
    this->colorDialog->hide();
    this->replot();
}
