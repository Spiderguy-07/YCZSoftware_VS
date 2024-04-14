#include "HistogramView.h"
#include <QPainter>
#include <QPaintEvent>
#include <algorithm>

HistogramView::HistogramView(QWidget* parent) : QWidget(parent) {
}

void HistogramView::setData(const QVector<double>& data, int numbers) {
    m_data = data;
    m_number = numbers;
    update(); // 请求重新绘制控件
}

void HistogramView::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    if (m_data.isEmpty())
    {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::white);
        // 为坐标轴和标签预留空间
        const int margin = 40;
        int width = this->width() - 2 * margin + 30;
        int height = this->height() - 2 * margin + 20;

        // 绘制坐标轴
        //painter.drawLine(margin, height + margin, width + margin, height + margin); // X轴
        //painter.drawLine(margin, margin, margin, height + margin); // Y轴
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    // 绘制背景
    painter.fillRect(rect(), Qt::white);

    // 为坐标轴和标签预留空间
    const int margin = 40;
    int width = this->width() - 2 * margin+30;
    int height = this->height() - 2 * margin;

    // 绘制坐标轴
    painter.drawLine(margin, height + margin, width + margin, height + margin); // X轴
    painter.drawLine(margin, margin, margin, height + margin); // Y轴

    // 计算数据范围和分段大小
    double min = *std::min_element(m_data.begin(), m_data.end());
    double max = *std::max_element(m_data.begin(), m_data.end());
    double range = max - min;
    if (range == 0) range = 1; // 避免除以0，如果所有值相同，则认为范围为1

    QVector<int> frequency(m_number, 0);
    double segmentSize = range / m_number;

    // 计算频数
    int maxFrequency = 0;
    for (double value : m_data) {
        int index = std::min(static_cast<int>((value - min) / segmentSize), m_number - 1);
        frequency[index]++;
        maxFrequency = std::max(maxFrequency, frequency[index]);
    }

    // 绘制直方图
    for (int i = 0; i < m_number; ++i) {
        double rectWidth = static_cast<double>(width) / m_number;
        double rectHeight = (static_cast<double>(frequency[i]) / maxFrequency) * height;
        QRectF bar(margin + i * rectWidth, height + margin - rectHeight, rectWidth, rectHeight);
        painter.fillRect(bar, Qt::blue);
        painter.drawRect(bar);

    }

    //painter.drawText(0, margin+heiht/4, margin - 5, 20, Qt::AlignRight, label);

    QFont font = painter.font();
    font.setPointSize(9); // 设置字号为10
    painter.setFont(font);
// 标注Y轴频率
    for (float i = 0; i <= 4; i++) { // 以最大频率的四分之一为步长绘制Y轴标签
        double height_i = height / 4 * i; // 这是格网线在画布上的垂直位置
        double freqRatio_i = maxFrequency * (i / 4); // 这实际上是分段内最大可能频数
        double proportion_i = freqRatio_i / m_data.size(); // 转换为对应的比例

        // 注意：在将频数转换为频率（即比例）时，我们使用总数据数来计算每一部分的比例
        QString label = QString::number(proportion_i, 'g', 2); // 使用'g'格式化可以避免不必要的0

        // 注意调整y坐标的计算，以确保标签正确放置
        int y = margin + height - static_cast<int>(height_i);
        painter.drawText(0, y - 10, margin - 3, 20, Qt::AlignRight, label);
    }

    // 绘制格网线和Y轴标签
    QPen gridPen(Qt::gray, 0, Qt::DashLine);
    painter.setPen(gridPen);
    for (float i = 0; i <= 4; i++) { // 以最大频率的四分之一为步长绘制Y轴标签
        double height_i = height / 4 * i;
        double frequency_i = maxFrequency / 4 * i;
        painter.drawLine(margin, margin + height - height_i, width + margin, margin + height - height_i);
    }
    painter.setPen(Qt::black);
    // X轴最小值和最大值的标注
    painter.drawText(margin-15, height + margin + 15, QString::number(min, 'f', 2));
    painter.drawText(width + margin - 40, height + margin + 15, QString::number(max, 'f', 2)); // 确保文本靠右绘制
}
