#ifndef INVERSELOGDIALOG_H
#define INVERSELOGDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qmessagebox.h>

namespace Ui {
class InverseLogDialog;
}

class InverseLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InverseLogDialog(QgsProject* project, QWidget *parent = nullptr);
    ~InverseLogDialog();

private:
    Ui::InverseLogDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void onCmbLayerChange();
    void onBtnDrawClicked();
};

#endif // INVERSELOGDIALOG_H
