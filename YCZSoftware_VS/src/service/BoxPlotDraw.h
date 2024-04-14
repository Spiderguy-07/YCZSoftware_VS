#ifndef BOXPLOTWIDGET_H
#define BOXPLOTWIDGET_H

#include <QWidget>
#include <QVector>
#include <cmath>
#include<iostream>

class BoxPlotDraw : public QWidget
{
    Q_OBJECT
public:
    explicit BoxPlotDraw(QWidget* parent = nullptr);

    // 设置数据
    void setData(const QVector<double>& data);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> m_data;

    // 边距
    int m_margin = 20;

    // 箱线图的位置和大小
    QRectF m_boxRect;

    // 计算箱线图的位置和大小
    double percentile(QVector<double>& data, double percent);
    double mean(const QVector<double> data);
    double standardDeviation(const QVector<double> data);
    double slopheight(double height, double range, double value, double bottomval);
};

#endif // BOXPLOTWIDGET_H
