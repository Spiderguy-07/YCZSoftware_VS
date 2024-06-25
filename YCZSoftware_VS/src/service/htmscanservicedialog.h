#ifndef HTMSCANSERVICEDIALOG_H
#define HTMSCANSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "HTMSCANPyThread.h"

namespace Ui {
class HTMSCANServiceDialog;
}

class HTMSCANServiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HTMSCANServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~HTMSCANServiceDialog();

private:
    Ui::HTMSCANServiceDialog* ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);
    int dim;

private slots:
    void InputChanged();
    void onCmbObPathChange();
    void onBtnBrowseNeiClicked();
    void onBtnBrowseOutputClicked();
    void onXYZChanged();
    void onBtnConfirmClicked();

signals:
    void sendPyParams(QList<double>observed_data, QList<ObPtXYZ> obpts, double length, QString neiPath, int id_sep, bool is_nei, QString outputPath, int dim, double Significant, int repeat_num);
    void getOutPath(QString, bool);
    void begin(QString);
};

#endif // HTMSCANSERVICEDIALOG_H
