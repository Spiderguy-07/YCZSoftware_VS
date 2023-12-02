#ifndef ABOUTSOFTDIALOG_H
#define ABOUTSOFTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutSoftDialog;
}

class AboutSoftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutSoftDialog(QWidget *parent = nullptr);
    ~AboutSoftDialog();

private:
    Ui::AboutSoftDialog *ui;
};

#endif // ABOUTSOFTDIALOG_H
