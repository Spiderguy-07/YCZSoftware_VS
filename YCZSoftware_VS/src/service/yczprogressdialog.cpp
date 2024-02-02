#include "yczprogressdialog.h"
#include "ui_yczprogressdialog.h"

yczprogressDialog::yczprogressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::yczprogressDialog)
{
    ui->setupUi(this);
    //setWindowTitle(name);
    connect(ui->cbxAutoClose, &QAbstractButton::toggled, this, &yczprogressDialog::onAutoCloseToggled);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

}

yczprogressDialog::~yczprogressDialog()
{
    delete ui;
}

void yczprogressDialog::reject()
{
    emit canceled();
    return QDialog::reject();
}

void yczprogressDialog::onAutoCloseToggled(bool checked)
{
    isAutoClose = checked;
}

void yczprogressDialog::onMessage(QString message)
{
    ui->progressMessage->setText(message);
    QLabel* logItem = new QLabel(message, ui->logScrollAreaContents);
    ui->logScrollAreaContents->layout()->addWidget(logItem);
}

void yczprogressDialog::onSuccess()
{
    ui->progressMessage->setText("Success");
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(100);
    if (isAutoClose)
    {
        return QDialog::accept();
    }
    else
    {
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel);
    }
}

void yczprogressDialog::onError(QString e)
{
    QLabel* logItem = new QLabel(e, ui->logScrollAreaContents);
    logItem->setPalette(QPalette(QPalette::WindowText, Qt::red));
    ui->logScrollAreaContents->layout()->addWidget(logItem);
    ui->progressMessage->setText(QStringLiteral("Error: ") + e);
}

