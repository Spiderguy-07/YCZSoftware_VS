// CoordinateAxes.cpp
#include "CoordinateAxes.h"
#include <QPainter>

CoordinateAxes::CoordinateAxes(QWidget* parent)
    : QWidget(parent) {
    // Customize as necessary
}

QSize CoordinateAxes::sizeHint() const {
    return QSize(100, 100); // Adjust the size as needed
}

void CoordinateAxes::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    // Render the axes here using QPainter
    // Example:
    painter.setPen(Qt::white);
    painter.drawLine(0, height(), width(), height()); // X axis
    painter.drawLine(0, height(), 0, 0);             // Y axis
    painter.drawLine(0, height(), width() / 2, 0);   // Z axis
}
