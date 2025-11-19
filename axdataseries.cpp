#include <QDebug>
#include <QRegularExpression>

#include "axdataseries.h"

AXDataSeries::AXDataSeries(QString variableName, QVector<double> xData, QVector<double> yData) {
    this->variableName = variableName;
    this->xData = xData;
    this->yData = yData;
    this->multipliedYData = yData;
    this->multiplier = 1.0;
    this->isDividing = false;
    this->isVisible = false;

    int r, g, b;
    r = 80 + qrand() % 120;
    g = 80 + qrand() % 120;
    b = 80 + qrand() % 120;
    this->color = QColor(r, g, b);
}

void AXDataSeries::trySetMultiplier(QString newMultiplier) {
    bool ok;
    double mult;

    mult = newMultiplier.toDouble(&ok);
    if (ok) {
        this->multiplier = mult;
        this->applyMultiplier();
    }
}

void AXDataSeries::applyMultiplier() {
    for (int i=0; i < this->yData.count(); i++) {
        if (this->isDividing && !(this->multiplier == 0.0)) {
            multipliedYData[i] = yData[i] / multiplier;
        } else if (!this->isDividing) {
            multipliedYData[i] = yData[i] * multiplier;
        }
    }
}
