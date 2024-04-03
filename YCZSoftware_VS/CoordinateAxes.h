// CoordinateAxes.h
#ifndef COORDINATEAXES_H
#define COORDINATEAXES_H

#include <QWidget>

class CoordinateAxes : public QWidget {
    Q_OBJECT
public:
    CoordinateAxes(QWidget* parent = nullptr);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    // Add any necessary variables or methods for rendering the axes
};

#endif // COORDINATEAXES_H
