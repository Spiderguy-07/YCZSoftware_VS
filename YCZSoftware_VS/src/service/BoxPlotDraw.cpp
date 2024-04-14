#include "BoxPlotDraw.h"
#include <QPainter>
#include <algorithm>

BoxPlotDraw::BoxPlotDraw(QWidget* parent) : QWidget(parent)
{
}

void BoxPlotDraw::setData(const QVector<double>& data)
{
    m_data = data;
    //calculateBoxRect();
    update();
}

void BoxPlotDraw::paintEvent(QPaintEvent* event) {

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    int m_t = 10;
    int m_b = 180;
    int width = this->width() - 10;
    int height = this->height() - m_t - m_b;

    // Copy data and sort
    QVector<double> sortedData = m_data;
    std::sort(sortedData.begin(), sortedData.end());
    // Calculate statistics
    double minVal = sortedData.first();
    double maxVal = sortedData.last();
    double median = percentile(sortedData,0.5);
    double q1 = percentile(sortedData, 0.25);
    double q3 = percentile(sortedData, 0.75);
    double IQR = q3 - q1;
    double meanval = mean(m_data);
    double sd = standardDeviation(m_data);
    
    double miniq = q1 - 1.5 * IQR;
    double maxiq = q3 + 1.5 * IQR;

    double topval = maxiq;
    if (sortedData.last() > maxiq)
    {
        topval = sortedData.last();
    }
    double bottomval = miniq;
    if (sortedData.first() < miniq)
    {
        bottomval = sortedData.first();
    }

    double range = topval - bottomval;

    // Draw box
    painter.setBrush(Qt::lightGray);
    int q3_pixel = m_t + slopheight(height, range, q3, bottomval);
    int q1_pixel = m_t + slopheight(height, range, q1, bottomval);
    painter.drawRect(75, q3_pixel, width - 90, q1_pixel - q3_pixel);

    // Draw median line
    painter.setPen(Qt::red);
    painter.drawLine(65, m_t + slopheight(height, range, median, bottomval), width-5, m_t + slopheight(height, range, median, bottomval));

    painter.setPen(Qt::black);
    // Draw whiskers
    painter.drawLine(95, m_t + slopheight(height, range, miniq , bottomval), width - 35, m_t + slopheight(height, range, miniq, bottomval));
    painter.drawLine(95, m_t + slopheight(height, range, maxiq, bottomval), width - 35, m_t + slopheight(height, range, maxiq, bottomval));

    painter.drawLine(width / 2 + 30, m_t + slopheight(height, range, miniq, bottomval), width / 2 + 30, m_t + slopheight(height, range, maxiq, bottomval));

    // Transform data coordinates to pixel coordinates
    QVector<QPointF> points;
    for (int i = 0; i < sortedData.size(); ++i) {
        double y = slopheight(height, range, sortedData[i], bottomval);
        double x = width / 2 + 30;
        points.append(QPointF(x, m_t + y));
    }

    // Draw scatter plot
    painter.setPen(Qt::blue); // Set pen color
    for (const QPointF& point : points) {
        painter.drawEllipse(point, 3, 3); // Draw point
    }
    // Draw outliers (not implemented in this example)

    painter.setPen(Qt::black); // Set pen color
        // Draw grid lines and ticks on Y-axis
    for (int i = 0; i <= 5; ++i) {
        double y = (topval - bottomval) / 5 * i + bottomval;
        double y_plot = slopheight(height, range, y, bottomval);

        // Draw Y-axis ticks
        QString yTickLabel = QString::number(y, 'f', 1);
        painter.setFont(QFont("Arial", 8)); // Set font size
        painter.drawText(5, m_t + y_plot + 5, yTickLabel);
    }
    painter.drawLine(40, m_t + slopheight(height, range, bottomval, bottomval), 40, slopheight(height, range, topval, bottomval));

    painter.drawText(5, m_t + height + 30, "min: ");
    painter.drawText(5, m_t + height + 50, "max: ");
    painter.drawText(5, m_t + height + 70, "Q1: ");
    painter.drawText(5, m_t + height + 90, "median: ");
    painter.drawText(5, m_t + height + 110, "Q3: ");
    painter.drawText(5, m_t + height + 130, "IQR: ");
    painter.drawText(5, m_t + height + 150, "mean: ");
    painter.drawText(5, m_t + height + 170, "s.d.: ");

    double x_t = 60; // 这是你原来计算的起始x位置，我们会基于此进行调整
    int startY = m_t + height + 30; // 初始的Y位置

    // 假设你有一个painter已经设置好了
    // QFontMetrics用于获取文本尺寸
    QFontMetrics fm(painter.font());

    // 一系列的值需要居中显示
    QList<double> values = { minVal, maxVal, q1, median, q3, IQR, meanval, sd };

    // 对于每个值，计算文本的宽度，并据此调整x位置，然后绘制文本
    for (int i = 0; i < values.size(); ++i) {
        QString text = QString::number(values[i], 'f', 4);
        int textWidth = fm.width(text);

        // 计算调整后的x位置以居中文本
        double adjusted_x_t = x_t + (width / 2 - textWidth / 2) - 30; // 假设width是绘制区域的总宽度

        // 绘制文本
        painter.drawText(adjusted_x_t, startY + 20 * i, text);
    }

    /*
    double x_t = width / 2 + 30;
    painter.drawText(x_t, m_t + height + 30, QString::number(minVal, 'f', 4));
    painter.drawText(x_t, m_t + height + 50, QString::number(maxVal, 'f', 4));
    painter.drawText(x_t, m_t + height + 70, QString::number(q1, 'f', 4));
    painter.drawText(x_t, m_t + height + 90, QString::number(median, 'f', 4));
    painter.drawText(x_t, m_t + height + 110, QString::number(q3, 'f', 4));
    painter.drawText(x_t, m_t + height + 130, QString::number(IQR, 'f', 4));
    painter.drawText(x_t, m_t + height + 150, QString::number(meanval, 'f', 4));
    painter.drawText(x_t, m_t + height + 170, QString::number(sd, 'f', 4));*/
}

double BoxPlotDraw::percentile(QVector<double>& data, double percent) {
    // 先对数据排序

    // 找到对应百分位的数据
    int n = data.size();
    double rank = (n - 1) * percent;
    int k = rank;
    double d = rank - k;

    if (k < 0) return data.first();
    if (k >= n - 1) return data.last();

    return data[k] + d * (data[k + 1] - data[k]);
}

double BoxPlotDraw::mean(const QVector<double> data)
{
    double sum = 0.0;
    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum / data.size();
}

double BoxPlotDraw::standardDeviation(const QVector<double> data)
{
    double avg = mean(data);
    double variance = 0.0;
    for (int i = 0; i < data.size(); ++i) {
        variance += pow(data[i] - avg, 2);
    }
    variance /= data.size();
    return sqrt(variance);
}

double BoxPlotDraw::slopheight(double height, double range, double value, double bottomval)
{
    return height - (height / range * (value - bottomval));
}
