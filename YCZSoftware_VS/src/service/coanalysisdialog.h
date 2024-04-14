#ifndef COANALYSISDIALOG_H
#define COANALYSISDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <cmath>
#include <qmessagebox.h>
#include "CorrelationMatrixWidget.h"

namespace Ui {
class CoAnalysisDialog;
}

class CoAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoAnalysisDialog(QgsProject* project, QWidget *parent = nullptr);
    ~CoAnalysisDialog();

private:
    Ui::CoAnalysisDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);
    double mean(const QVector<double>& vec);
    double pearsonCorrelation(const QVector<double>& x, const QVector<double>& y);
    QVector<QVector<double>> computeCorrelationMatrix(const QVector<QVector<double>>& data);

private slots:
    void onCmbLayerChange();
    void onCmbNumChange();
    void onBtnOkClicked();
};

#endif // COANALYSISDIALOG_H
