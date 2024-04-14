#ifndef CORRELATIONMATRIXWIDGET_H
#define CORRELATIONMATRIXWIDGET_H

#include <QWidget>
#include <QVector>
#include <QStringList>

class CorrelationMatrixWidget : public QWidget {
    Q_OBJECT
public:
    explicit CorrelationMatrixWidget(QWidget* parent = nullptr);
    void setCorrelationMatrix(const QVector<QVector<double>>& matrix);
    void setLabels(const QStringList& labels);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QVector<double>> correlationMatrix;
    QStringList labels;
    int margin = 50; // ±ß¾à£¬¿Éµ÷Õû
    QColor colorForValue(double value) const;
    void drawColorBar(QPainter& painter);
};

#endif // CORRELATIONMATRIXWIDGET_H
