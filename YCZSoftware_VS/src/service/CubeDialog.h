#ifndef CUBEDIALOG_H
#define CUBEDIALOG_H

#include <QDialog>

namespace Ui {
class CubeDialog;
}

class CubeDialog : public QDialog
{
    Q_OBJECT

signals:
    void SendCube(int index, double size);

public:
    explicit CubeDialog(QWidget *parent = nullptr);
    ~CubeDialog();

private:
    Ui::CubeDialog *ui;


private slots:
    void onActionInput();
    void onActionConfirm();
    void onActionCancel();
};

#endif // CUBEDIALOG_H
