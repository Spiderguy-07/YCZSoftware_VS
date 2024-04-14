// qqplotwidget.cpp
#include "QQPlotDraw.h"
#include <QPainter>
#include <algorithm>
#include <iostream>

QQPlotDraw::QQPlotDraw(QWidget* parent) : QWidget(parent)
{
}

void QQPlotDraw::setData(const QVector<double>& data)
{
    m_data = data;
    update(); // 重新绘制图形
}
/*
void QQPlotDraw::paintEvent(QPaintEvent* event) {

    if (m_data.isEmpty()) return;

    // Copy data and sort
    QVector<double> sortedData = m_data;
    std::sort(sortedData.begin(), sortedData.end());

    // Calculate mean and standard deviation
    double avg_data = mean(sortedData);
    double std_data = standardDeviation(sortedData);

    // Find min and max values
    double minValue = sortedData.first();
    double maxValue = sortedData.last();

    // Calculate z values
    QVector<double> z_real;
    for (int i = 1; i <= sortedData.size(); ++i) {
        double n = sortedData.size();
        double id = i;

        double cdf = (id - 0.3) / (n + 0.4);
        double z = calculateZ(cdf);

        z_real.append(z);
    }

    // Create a QPainter and set up pen and font
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up coordinate system
    int margin = 30;
    int xAxisLength = width() - 2 * margin;
    int yAxisLength = height() - 2 * margin;

    // Fill background with white color
    painter.fillRect(rect(), Qt::white);

    // Draw border
    painter.setPen(Qt::black);
    painter.drawRect(margin, margin, xAxisLength, yAxisLength);

    // Draw grid lines and ticks on X-axis
    for (int i = 0; i <= 10; ++i) {
        double x = mapValue(i / 10.0, 0, 1, margin, width() - margin);

        // Set pen for grid lines
        QPen gridPen(Qt::gray, 0, Qt::DashLine);
        painter.setPen(gridPen);

        painter.drawLine(x, height() - margin, x, margin); // Vertical grid line

        painter.setPen(Qt::black);
        // Draw X-axis ticks
        QString xTickLabel = QString::number(mapValue(i / 10.0, 0, 1, z_real.first(), z_real.last()), 'f', 1);
        painter.setFont(QFont("Arial", 8)); // Set font size
        painter.drawText(x - 10, height() - margin / 2 , xTickLabel);
    }

    // Draw grid lines and ticks on Y-axis
    for (int i = 0; i <= 10; ++i) {
        double y = mapValue(i / 10.0, 0, 1, height() - margin, margin);

        // Set pen for grid lines
        QPen gridPen(Qt::gray, 0, Qt::DashLine);
        painter.setPen(gridPen);

        painter.drawLine(margin, y, width() - margin, y); // Horizontal grid line

        painter.setPen(Qt::black);
        // Draw Y-axis ticks
        QString yTickLabel = QString::number(mapValue(i / 10.0, 0, 1, minValue, maxValue), 'f', 1);
        painter.setFont(QFont("Arial", 8)); // Set font size
        painter.drawText(margin / 2 - 10, y + 5, yTickLabel);
    }


    // Transform data coordinates to pixel coordinates
    QVector<QPointF> points;
    for (int i = 0; i < sortedData.size(); ++i) {
        double x = mapValue(z_real[i], z_real.first(), z_real.last(), margin, width() - margin);
        double y = mapValue(sortedData[i], minValue, maxValue, height() - margin, margin);
        points.append(QPointF(x, y));
    }

    // Draw scatter plot
    painter.setPen(Qt::blue); // Set pen color
    for (const QPointF& point : points) {
        painter.drawEllipse(point, 3, 3); // Draw point
    }

    // Draw line
    double x_s = z_real.first();
    double y_s = function(std_data, avg_data, x_s);
    if (y_s < minValue)
    {
        x_s = (minValue - avg_data) / std_data;
        y_s = minValue;
    }

    double x_e = z_real.last();
    double y_e = function(std_data, avg_data, x_e);
    if (y_e > maxValue)
    {
        x_e = (maxValue - avg_data) / std_data;
        y_e = maxValue;
    }
    QPointF startPoint(mapValue(x_s, z_real.first(), z_real.last(), margin, width() - margin), mapValue(y_s, minValue, maxValue, height() - margin, margin));
    QPointF endPoint(mapValue(x_e, z_real.first(), z_real.last(), margin, width() - margin), mapValue(y_e, minValue, maxValue, height() - margin, margin));
    // Set pen for line
    QPen linePen(Qt::black, 2); // Set width to 2
    painter.setPen(linePen);

    painter.drawLine(startPoint, endPoint);
}
*/

void QQPlotDraw::paintEvent(QPaintEvent* event) {

    if (m_data.isEmpty()) return;

    QVector<double> sortedData = m_data;
    std::sort(sortedData.begin(), sortedData.end());

    double avg_data = mean(sortedData);
    double std_data = standardDeviation(sortedData);

    double minValue = sortedData.first();
    double maxValue = sortedData.last();

    QVector<double> z_real;
    for (int i = 1; i <= sortedData.size(); ++i) {
        double n = sortedData.size();
        double cdf = (i - 0.3) / (n + 0.4);
        double z = calculateZ(cdf);
        z_real.append(z);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Adjusted margins
    int leftMargin = 50;  // Increase left margin
    int rightMargin = 15; // Decrease right margin
    int topMargin = 25;   // Normal top margin
    int bottomMargin = 40;// Normal bottom margin

    // Adjusted axis lengths based on new margins
    int xAxisLength = width() - leftMargin - rightMargin;
    int yAxisLength = height() - topMargin - bottomMargin;

    painter.fillRect(rect(), Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(leftMargin, topMargin, xAxisLength, yAxisLength);

    for (int i = 0; i <= 10; ++i) {
        double x = mapValue(i / 10.0, 0, 1, leftMargin, width() - rightMargin);

        QPen gridPen(Qt::gray, 0, Qt::DashLine);
        painter.setPen(gridPen);

        painter.drawLine(x, height() - bottomMargin, x, topMargin);

        painter.setPen(Qt::black);
        QString xTickLabel = QString::number(mapValue(i / 10.0, 0, 1, z_real.first(), z_real.last()), 'f', 1);
        painter.setFont(QFont("Arial", 8));
        painter.drawText(x - 10, height() - bottomMargin / 2, xTickLabel);
    }

    for (int i = 0; i <= 10; ++i) {
        double y = mapValue(i / 10.0, 0, 1, height() - bottomMargin, topMargin);

        QPen gridPen(Qt::gray, 0, Qt::DashLine);
        painter.setPen(gridPen);

        painter.drawLine(leftMargin, y, width() - rightMargin, y);

        painter.setPen(Qt::black);
        QString yTickLabel = QString::number(mapValue(i / 10.0, 0, 1, minValue, maxValue), 'f', 1);
        painter.setFont(QFont("Arial", 8));
        painter.drawText(leftMargin / 2 - 10, y + 5, yTickLabel);
    }

    QVector<QPointF> points;
    for (int i = 0; i < sortedData.size(); ++i) {
        double x = mapValue(z_real[i], z_real.first(), z_real.last(), leftMargin, width() - rightMargin);
        double y = mapValue(sortedData[i], minValue, maxValue, height() - bottomMargin, topMargin);
        points.append(QPointF(x, y));
    }

    painter.setPen(Qt::blue);
    for (const QPointF& point : points) {
        painter.drawEllipse(point, 3, 3);
    }

    double x_s = z_real.first();
    double y_s = function(std_data, avg_data, x_s);
    if (y_s < minValue) {
        x_s = (minValue - avg_data) / std_data;
        y_s = minValue;
    }

    double x_e = z_real.last();
    double y_e = function(std_data, avg_data, x_e);
    if (y_e > maxValue) {
        x_e = (maxValue - avg_data) / std_data;
        y_e = maxValue;
    }
    QPointF startPoint(mapValue(x_s, z_real.first(), z_real.last(), leftMargin, width() - rightMargin), mapValue(y_s, minValue, maxValue, height() - bottomMargin, topMargin));
    QPointF endPoint(mapValue(x_e, z_real.first(), z_real.last(), leftMargin, width() - rightMargin), mapValue(y_e, minValue, maxValue, height() - bottomMargin, topMargin));

    QPen linePen(Qt::black, 2);
    painter.setPen(linePen);

    painter.drawLine(startPoint, endPoint);
}

// 计算误差函数
double QQPlotDraw::erf(double x) {
    return std::erf(x);
}

// 计算误差函数的导数
double QQPlotDraw::erfDerivative(double x) {
    return 2.0 / std::sqrt(M_PI) * std::exp(-x * x);
}

// 牛顿迭代法计算逆误差函数的近似值
double QQPlotDraw::erfinv(double y) {
    const double EPSILON = 1e-15; // 迭代的精度
    double x = 0.0;
    double dx;

    do {
        dx = (erf(x) - y) / erfDerivative(x);
        x -= dx;
    } while (std::abs(dx) > EPSILON);

    return x;
}

double QQPlotDraw::calculateZ(double cdf) {
    // 这里使用近似方法计算逆误差函数的近似值
    return std::sqrt(2.0) * erfinv(2 * cdf - 1);
}

double QQPlotDraw::mean(const QVector<double> data)
{
    double sum = 0.0;
    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum / data.size();
}

double QQPlotDraw::standardDeviation(const QVector<double> data)
{
    double avg = mean(data);
    double variance = 0.0;
    for (int i = 0; i < data.size(); ++i) {
        variance += pow(data[i] - avg, 2);
    }
    variance /= data.size();
    return sqrt(variance);
}

double QQPlotDraw::calculateCDF(double k, double n)
{
    return (k - 0.3) / (n + 0.4);
}

// Helper function to map values from one range to another
double QQPlotDraw::mapValue(double value, double inMin, double inMax, double outMin, double outMax) {
    return (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

double QQPlotDraw::function(double a, double b, double x)
{
    return a * x + b;
}
