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
    }

    this->scrollVStack->addStretch();
}


void GraphsView::deletePlots() {
    clearLayout(this->scrollVStack);
}

void GraphsView::setPlotsXRange(int xMin, int yMin) {
    return;
}

void clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {

        if (QWidget* widget = item->widget())
            widget->deleteLater();

        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout);

        delete item;
    }
}
