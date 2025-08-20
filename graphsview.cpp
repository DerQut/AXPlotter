#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

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

    QPushButton* forceUpdateButton = new QPushButton("Update", this);
    connect(forceUpdateButton, &QPushButton::clicked, this, [this]() {
        this->updatePlots(this->contentView->axinterpreter->baseFolder.absolutePath());
        qDebug() << this->contentView->axinterpreter->baseFolder.absolutePath();
    });
    mainVStack->addWidget(forceUpdateButton);

    mainVStack->addWidget(scrollArea);
}


void GraphsView::updatePlots(QString directoryName) {

    // Remove all widgets from the VStack
    QLayoutItem* itemToRemove;
    while ((itemToRemove = this->scrollVStack->takeAt(0)) != nullptr) {
        if (itemToRemove->widget()) {
            delete itemToRemove->widget();
        }
        delete itemToRemove;
    }


    // Read all .csv files
    QDir csvDir (directoryName);
    QStringList csvFiles = csvDir.entryList(QStringList() << "*.csv", QDir::Files);

    if (csvFiles.isEmpty()) {
        QLabel* failedLabel = new QLabel("No files found in:", this);
        this->scrollVStack->addWidget(failedLabel);
        return;
    }

    // Add the graphs

    QCPMarginGroup* marginGroup = new QCPMarginGroup(nullptr);

    foreach (const QString &filename, csvFiles) {
        QLabel* graphTitle = new QLabel(filename, this);
        graphTitle->setAlignment(Qt::AlignCenter);

        QCustomPlot* plot = new QCustomPlot();
        plot->setMinimumHeight(200);
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

        this->scrollVStack->addWidget(graphTitle);
        this->scrollVStack->addWidget(plot);

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
                }
            }
            csvFile.close();
        }

        plot->addGraph();
        plot->graph(0)->addData(xData, yData);

        plot->yAxis->rescale();
        plot->xAxis->rescale();

        plot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    }

    QLabel* dirLabel = new QLabel(directoryName, this);

    this->scrollVStack->addWidget(dirLabel);

    this->scrollVStack->addStretch();
}

void GraphsView::setPlotsXRange(int xMin, int yMin) {
    return;
}
