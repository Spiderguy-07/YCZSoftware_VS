// qqplotwidget.h
#ifndef QQPLOTWIDGET_H
#define QQPLOTWIDGET_H

#include <QWidget>
#include <QVector>
#include <cmath>

class QQPlotDraw : public QWidget
{
    Q_OBJECT
public:
    explicit QQPlotDraw(QWidget* parent = nullptr);

    void setData(const QVector<double>& data);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> m_data;
    double calculateCDF(double k, double n);
    double erf(double x);
    double erfDerivative(double x);
    double erfinv(double y);
    double calculateZ(double cdf);
    double mean(const QVector<double> data);
    double standardDeviation(const QVector<double> data);
    double mapValue(double value, double inMin, double inMax, double outMin, double outMax);
    double function(double a, double b, double x);

};

#endif // QQPLOTWIDGET_H