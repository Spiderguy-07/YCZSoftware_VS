#include "CorrelationMatrixWidget.h"
#include <QPainter>
#include <QPaintEvent>

CorrelationMatrixWidget::CorrelationMatrixWidget(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(400, 300); // 调整最小尺寸以确保色带有足够的空间显示
}

void CorrelationMatrixWidget::setCorrelationMatrix(const QVector<QVector<double>>& matrix) {
    correlationMatrix = matrix;
    update(); // 请求重绘
}

void CorrelationMatrixWidget::setLabels(const QStringList& labels) {
    this->labels = labels;
    update();
}

void CorrelationMatrixWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    if (correlationMatrix.isEmpty())
        return;

    int n = correlationMatrix.size();
    int cellWidth = (width() - 2 * margin - 50) / n; // 留出50像素宽度给色带
    int cellHeight = (height() - 2 * margin) / n;

    // 绘制矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            painter.setFont(QFont("Arial", 8));
            double value = correlationMatrix[i][j];
            QColor color = colorForValue(value);
            QRect cellRect(margin + j * cellWidth, margin + i * cellHeight, cellWidth, cellHeight);
            painter.fillRect(cellRect, color);
            painter.setPen(Qt::black);
            painter.drawText(cellRect, Qt::AlignCenter, QString::number(value, 'f', 2));

            if (i == 0) {
                painter.setFont(QFont("Arial", 8));
                painter.drawText(QRect(margin + j * cellWidth, 15, cellWidth, margin), Qt::AlignCenter, labels[j]);
            }
        }
        painter.setFont(QFont("Arial", 8));
        painter.drawText(QRect(15, margin + i * cellHeight, margin, cellHeight), Qt::AlignCenter, labels[i]);
    }

    // 绘制色带
    drawColorBar(painter);
}

QColor CorrelationMatrixWidget::colorForValue(double value) const {
    // 映射颜色值：蓝色 (-1) 到 白色 (0) 到红色 (1)
    if (value < 0) {
        // 从蓝色到白色
        int t = static_cast<int>((value + 1) * 155);  // value + 1 是 0 到 1 的范围
        return QColor(100+t, 100+t, 255);
    } else {
        // 从白色到红色
        int t = static_cast<int>(value * 155);  // value 是 0 到 1 的范围
        return QColor(255, 255 - t, 255 - t);
    }
}
void CorrelationMatrixWidget::drawColorBar(QPainter& painter) {
    int barWidth = 20;
    int barHeight = height() - 2 * margin;
    QRect barRect(width() - margin - barWidth, margin, barWidth, barHeight);
    QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());

    // 从蓝色到白色到红色的渐变
    gradient.setColorAt(0.0, QColor(255, 100, 100));  // 温和的红色
    gradient.setColorAt(0.5, QColor(255, 255, 255));  // 白色
    gradient.setColorAt(1.0, QColor(100, 100, 255));  // 蓝色

    painter.fillRect(barRect, gradient);

    // 绘制刻度线和标注
    int numTicks = 5;
    painter.setPen(Qt::black);
    for (int i = 0; i < numTicks; ++i) {
        double value = -1 + i * 2.0 / (numTicks - 1); // 计算刻度的相关系数值
        int y = margin + static_cast<int>(barHeight * (i * 1.0 / (numTicks - 1)));

        // 绘制刻度线
        painter.drawLine(barRect.right() + 1, y, barRect.right() + 5, y);

        // 绘制标注
        painter.drawText(barRect.right() + 10, y + painter.fontMetrics().height() / 3, QString::number(value, 'f', 1));
    }
}
