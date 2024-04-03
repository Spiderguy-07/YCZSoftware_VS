// colorwidget.cpp
#include "ColorWidget.h"
#include <QPainter>

ColorWidget::ColorWidget(QWidget* parent) : QWidget(parent), m_color(Qt::black)
{
    setFixedSize(150, 45); // 设置控件固定大小
}

void ColorWidget::setColor(const QColor& color)
{
    if (m_color != color) {
        m_color = color;
        update(); // 更新控件以便重绘
    }
}

void ColorWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), m_color); // 用指定颜色填充控件区域
}
