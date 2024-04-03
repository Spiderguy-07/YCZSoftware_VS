#include "CubeDialog.h"
#include "ui_CubeDialog.h"

CubeDialog::CubeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CubeDialog)
{
    ui->setupUi(this);
    ui->cmbType->addItem("Point");
    ui->cmbType->addItem("Cube");
    ui->dsbSize->setEnabled(false);

    this->connect(ui->cmbType, SIGNAL(currentIndexChanged(int)), this, SLOT(onActionInput()));
    this->connect(ui->btnOK, &QAbstractButton::clicked, this, &CubeDialog::onActionConfirm);
    this->connect(ui->btnCancel, &QAbstractButton::clicked, this, &CubeDialog::onActionCancel);
}

CubeDialog::~CubeDialog()
{
    delete ui;
}

void CubeDialog::onActionInput()
{
    if (ui->cmbType->currentIndex() == 1)
    {
        ui->dsbSize->setEnabled(true);
    }
    else
    {
        ui->dsbSize->setEnabled(false);
    }
}

void CubeDialog::onActionConfirm()
{
    int index_type = 0;
    if (ui->cmbType->currentIndex() == 0)
    {
        index_type = 1;
    }
    else if (ui->cmbType->currentIndex() == 1)
    {
        index_type = 4;
    }
    else
    {

    }
    double a = ui->dsbSize->value();
    //std::cout << ui->dsbSize->value();
    emit SendCube(index_type, ui->dsbSize->value());
    this->close();
}

void CubeDialog::onActionCancel()
{
    this->close();
}
