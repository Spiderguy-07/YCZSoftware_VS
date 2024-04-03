// colorwidget.h
#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QColor>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget* parent = nullptr);
    void setColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QColor m_color;
};

#endif // COLORWIDGET_H
