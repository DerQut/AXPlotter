#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QSlider>

#include "graphsview.h"
#include "contentview.h"
#include "axinterpreter.h"

GraphsView::GraphsView(ContentView *parent) :
    QWidget(parent)
{
    this->contentView = parent;

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* mainVStack = new QVBoxLayout(this);
    mainVStack->setContentsMargins(0, 0, 0, 0);
    mainVStack->setSpacing(0);

    QWidget* scrollWidget = new QWidget(this);
    scrollArea->setWidget(scrollWidget);

    this->scrollVStack = new QVBoxLayout(scrollWidget);

    mainVStack->addWidget(scrollArea);

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

    connect(this->xMinSlider, SIGNAL(valueChanged(int)), this, SLOT(xMinSliderCall()));
    connect(this->xMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(xMaxSliderCall()));

}


void GraphsView::updatePlots(QString directoryName) {

    // Clear the plots
    this->deletePlots();

    // Read all .csv files
    QDir csvDir (directoryName);
    QStringList csvFiles = csvDir.entryList(QStringList() << "*.csv", QDir::Files);

    if (csvFiles.isEmpty()) {
        QLabel* failedLabel = new QLabel("No files found in: " +this->contentView->axinterpreter->baseFolder.absolutePath(), this);
        this->scrollVStack->addWidget(failedLabel);
        return;
    }

    // Read the max timestep value
    double maxTimestep = 0.0;
    QFile timestepFile(directoryName + QDir::separator()+ "timestep.txt");
    if (timestepFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream timeIn(&timestepFile);
        maxTimestep = timeIn.readLine().toDouble();
    }

    // Add the graphs

    QCPMarginGroup* marginGroup = new QCPMarginGroup(nullptr);

    foreach (const QString &filename, csvFiles) {

        QString cleanFilename = filename;
        cleanFilename.remove(QRegularExpression("[\\.][cC][sS][vV]"));
        cleanFilename.replace(QRegularExpression("\\_+"), " ");

        QHBoxLayout* mainHStack = new QHBoxLayout();

        QLabel* graphTitle = new QLabel(cleanFilename, this);
        graphTitle->setAlignment(Qt::AlignCenter);
        graphTitle->setWordWrap(true);
        graphTitle->setFixedWidth(100);
        mainHStack->addWidget(graphTitle);

        QCustomPlot* plot = new QCustomPlot();
        plot->setMinimumHeight(200);
        mainHStack->addWidget(plot);

        this->scrollVStack->addLayout(mainHStack);

        QVector<double> xData, yData;
        QFile csvFile (csvDir.absoluteFilePath(filename));

        if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in (&csvFile);

            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList splitLine = line.split(',');

                if (splitLine.size() > 1) {
                    xData.append(splitLine[0].toDouble());
                    yData.append(splitLine[1].toDouble());

                    // Duplicate the same Y value for the end of the run
                    if (in.atEnd() && splitLine[0].toDouble() < maxTimestep) {
                        xData.append(maxTimestep);
                        yData.append(splitLine[1].toDouble());
                    }
                }
            }
            csvFile.close();
        }

        QCPCurve* curve = new QCPCurve(plot->xAxis, plot->yAxis);
        curve->addData(xData, yData);

        plot->yAxis->rescale();
        plot->xAxis->rescale();

        plot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);

        this->plots.append(plot);
    }

    this->scrollVStack->addStretch();

    this->xMaxSlider->setRange(0, maxTimestep);
    this->xMaxSlider->setValue(maxTimestep);

    this->xMinSlider->setRange(0, maxTimestep);
    this->xMinSlider->setValue(0);
}


void GraphsView::deletePlots() {
    clearLayout(this->scrollVStack);
    this->plots.clear();
}

void GraphsView::setPlotsXRange(int xMin, int xMax) {
    foreach (QCustomPlot* plot, this->plots) {
        plot->xAxis->setRange(xMin, xMax);
        for (int i=0; i < plot->graphCount(); i++) {
            plot->graph(i)->rescaleValueAxis(false, true);
        }
        plot->replot();
    }
}

void GraphsView::updatePlotsRange() {
    qDebug() << "xMin: " << this->xMinSlider->value() << ", xMax: " << this->xMaxSlider->value();
    this->setPlotsXRange(this->xMinSlider->value(), this->xMaxSlider->value());
}

void GraphsView::xMaxSliderCall() {
    this->xMaxLabel->setText("X range (max): " + QString::number(this->xMaxSlider->value()));
    if (xMaxSlider->value() < xMinSlider->value()) {
        xMinSlider->setValue(xMaxSlider->value());
    }
    this->updatePlotsRange();
}

void GraphsView::xMinSliderCall() {
    this->xMinLabel->setText("X range (min): " + QString::number(this->xMinSlider->value()));
    if (xMinSlider->value() > xMaxSlider->value()) {
        xMaxSlider->setValue(xMinSlider->value());
    }
    this->updatePlotsRange();
}

void clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {

        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }

        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }

        delete item;
    }
}
