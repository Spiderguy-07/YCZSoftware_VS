#ifndef QQPLOTDIALOG_H
#define QQPLOTDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qmessagebox.h>
#include "QQPlotDraw.h"

namespace Ui {
class QQPlotDialog;
}

class QQPlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QQPlotDialog(QgsProject* project, QWidget *parent = nullptr);
    ~QQPlotDialog();

private:
    Ui::QQPlotDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void onCmbLayerChange();
    void onBtnDrawClicked();
};

#endif // QQPLOTDIALOG_H
