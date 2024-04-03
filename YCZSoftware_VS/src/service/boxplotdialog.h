#ifndef BOXPLOTDIALOG_H
#define BOXPLOTDIALOG_H

#include <QDialog>

namespace Ui {
class BoxPlotDialog;
}

class BoxPlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoxPlotDialog(QWidget *parent = nullptr);
    ~BoxPlotDialog();

private:
    Ui::BoxPlotDialog *ui;
};

#endif // BOXPLOTDIALOG_H
