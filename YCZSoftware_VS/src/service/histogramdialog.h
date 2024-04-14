#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qmessagebox.h>
#include "HistogramView.h"

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(QgsProject* project, QWidget *parent = nullptr);
    ~HistogramDialog();

private:
    Ui::HistogramDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void onCmbLayerChange();
    void onBtnDrawClicked();
};

#endif // HISTOGRAMDIALOG_H
