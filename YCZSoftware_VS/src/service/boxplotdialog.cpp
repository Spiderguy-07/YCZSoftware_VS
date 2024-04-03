#include "boxplotdialog.h"
#include "ui_boxplotdialog.h"

BoxPlotDialog::BoxPlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxPlotDialog)
{
    ui->setupUi(this);
}

BoxPlotDialog::~BoxPlotDialog()
{
    delete ui;
}
