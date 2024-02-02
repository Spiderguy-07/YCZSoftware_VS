#ifndef YCZPROGRESSDIALOG_H
#define YCZPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class yczprogressDialog;
}

class yczprogressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit yczprogressDialog(QWidget *parent = nullptr);
    ~yczprogressDialog();

    virtual void reject() override;

private:
    Ui::yczprogressDialog *ui;
    bool isAutoClose = true;

public:
    //void onTick(int current, int total);
    void onMessage(QString message);
    void onSuccess();
    //void onCanceled();
    void onError(QString e);
    //void onPlot(QVariant data, PlotFunction func);

public slots:
    void onAutoCloseToggled(bool checked);

signals:
    void canceled();

};

#endif // YCZPROGRESSDIALOG_H
