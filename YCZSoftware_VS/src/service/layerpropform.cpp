#include "layerpropform.h"
#include "ui_layerpropform.h"

LayerPropForm::LayerPropForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerPropForm)
{
    ui->setupUi(this);
}

LayerPropForm::~LayerPropForm()
{
    delete ui;
}
