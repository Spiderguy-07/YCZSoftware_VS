#ifndef LAYERPROPFORM_H
#define LAYERPROPFORM_H

#include <QWidget>

namespace Ui {
class LayerPropForm;
}

class LayerPropForm : public QWidget
{
    Q_OBJECT

public:
    explicit LayerPropForm(QWidget *parent = nullptr);
    ~LayerPropForm();

private:
    Ui::LayerPropForm *ui;
};

#endif // LAYERPROPFORM_H
