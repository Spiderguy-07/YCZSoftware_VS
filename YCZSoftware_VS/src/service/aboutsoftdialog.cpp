#include "aboutsoftdialog.h"
#include "ui_aboutsoftdialog.h"

AboutSoftDialog::AboutSoftDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutSoftDialog)
{
    ui->setupUi(this);
    this->show();
}

AboutSoftDialog::~AboutSoftDialog()
{
    delete ui;
}
