#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QVector>

class HistogramView : public QWidget {
    Q_OBJECT

public:
    explicit HistogramView(QWidget* parent = nullptr);

    // 设置数据
    void setData(const QVector<double>& data, int numbers);

protected:
    // 重写绘制事件
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> m_data; // 存储直方图数据
    int m_number;
};

#endif // HISTOGRAMWIDGET_H
