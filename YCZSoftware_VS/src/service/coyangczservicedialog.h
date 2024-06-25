#ifndef COYANGCZSERVICEDIALOG_H
#define COYANGCZSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "CoYangCZPyThread.h"

namespace Ui {
class CoYangCZServiceDialog;
}

class CoYangCZServiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoYangCZServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~CoYangCZServiceDialog();

private:
    Ui::CoYangCZServiceDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);
    int dim;
    bool isTIF;

signals:
    void sendPyParams(QList<double> observed_data, QList<ObPtXYZ> co_observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> co_obpts, QList<ObPtXYZ> un_obpts, QList<double> c_list, int k, int dim, int n_co, QString outputPath, bool isnugget, bool isTIF);
    void getOutPath(QString, bool);
    void begin(QString);

private slots:
    void InputChanged();
    void onCmbObPathChange();
    void onCmbUnknowChange();
    void onCmbCo1Change();
    void onCmbCo2Change();
    void onCmbCo3Change();
    void onXYZChanged();
    void onBtnBrowseOutputClicked();
    void onBtnConfirmClicked();
};

#endif // COYANGCZSERVICEDIALOG_H
