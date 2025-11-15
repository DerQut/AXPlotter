#ifndef AXDATASERIES_H
#define AXDATASERIES_H

#include <QString>
#include <QVector>
#include <QColor>


class AXDataSeries {
public:
    explicit AXDataSeries(QString variableName, QVector<double> xData, QVector<double> yData);

    void trySetMultiplier(QString newMultiplier);
    void applyMultiplier();

    QString variableName;
    QColor color;

    QVector<double> xData;
    QVector<double> yData;
    QVector<double> multipliedYData;

    double multiplier;

};

#endif // AXDATASERIES_H
