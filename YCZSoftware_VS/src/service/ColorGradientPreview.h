#ifndef COLORGRADIENTPREVIEW_H
#define COLORGRADIENTPREVIEW_H

#include <QWidget>
#include <QColor>

class ColorGradientPreview : public QWidget {
    Q_OBJECT
public:
    explicit ColorGradientPreview(QWidget* parent = nullptr);

    void setGradientColors(const QColor& start, const QColor& end);
    void setNumSteps(int steps);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QColor startColor;
    QColor endColor;
    int numSteps;

    void calculateColorGradient(const QColor& start, const QColor& end, int numSteps, QVector<QColor>& gradient);
};

#endif // COLORGRADIENTPREVIEW_H
