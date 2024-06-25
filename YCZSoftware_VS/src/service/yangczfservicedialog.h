#ifndef YANGCZFSERVICEDIALOG_H
#define YANGCZFSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "YangCZFPyThread.h"

namespace Ui {
class YangCZFServiceDialog;
}

class YangCZFServiceDialog : public QDialog
{
    Q_OBJECT

signals:
    void sendPyParams(QList<double>observed_data, QList<ObPtXYZ> obpts, double c, int k, int dim);
    void begin(QString);

public:
    explicit YangCZFServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~YangCZFServiceDialog();

private:
    int dim;
    Ui::YangCZFServiceDialog* ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr); 

private slots:
    void onCmbObPathChange();
    void onXYZChanged();
    void onBtnConfirmClicked();
};

#endif // YANGCZFSERVICEDIALOG_H
