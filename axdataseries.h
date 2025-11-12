#ifndef AXDATASERIES_H
#define AXDATASERIES_H

#include <QString>
#include <QVector>


class AXDataSeries {
public:
    explicit AXDataSeries(QString variableName, QVector<double> xData, QVector<double> yData);

    QString variableName;

    QVector<double> xData;
    QVector<double> yData;
    QVector<double> multipliedYData;

    double multiplier;

};

#endif // AXDATASERIES_H
