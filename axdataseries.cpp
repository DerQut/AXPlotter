#include "axdataseries.h"

AXDataSeries::AXDataSeries(QString variableName, QVector<double> xData, QVector<double> yData) {
    this->variableName = variableName;
    this->xData = xData;
    this->yData = yData;
    this->multipliedYData = yData;
    this->multiplier = 1.0;
}
