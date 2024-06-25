#ifndef STYANGCZSERVICEDIALOG_H
#define STYANGCZSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "STYangCZPyThread.h"

namespace Ui {
class STYangCZServiceDialog;
}

class STYangCZServiceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit STYangCZServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~STYangCZServiceDialog();

private:
    int dim;
    Ui::STYangCZServiceDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void InputChanged();
    void onCmbObPathChange();
    void onBtnBrowseOutputClicked();
    void onXYZChanged();
    void onBtnConfirmClicked();
    void onBtnCancelClicked();
signals:
    void begin(QString);
    void sendPyParams(QList<QList<double>>observed_data, QList<ObPtXYZ> obpts, double c_s, double c_t, int k_s, int k_t, int dim, QString outputPath, bool e);
    void getOutPath(QString, bool);
};

#endif // STYANGCZSERVICEDIALOG_H
