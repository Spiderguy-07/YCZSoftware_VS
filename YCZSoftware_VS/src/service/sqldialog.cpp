#include "sqldialog.h"
#include "ui_sqldialog.h"

SQLDialog::SQLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLDialog)
{
    ui->setupUi(this);
    this->show();
}

SQLDialog::~SQLDialog()
{
    delete ui;
}
