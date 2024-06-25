#ifndef GYANGCZSERVICEDIALOG_H
#define GYANGCZSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "GYangCZPyThread.h"

namespace Ui {
class GYangCZServiceDialog;
}

class GYangCZServiceDialog : public QDialog
{
    Q_OBJECT
signals:
    void begin(QString);
    void sendPyParams(QList<double>observed_data, QList<ObPtXYZ> obpts, QList<double>train_index_matrix, QList<double>test_index_matrix, QList<ObPtXYZ> un_obpts, double c, int k, int k2, int dim, QString outputPath, bool e, bool isTIF);
    void getOutPath(QString, bool);

public:
    explicit GYangCZServiceDialog(QgsProject* project, QWidget* parent = nullptr);
    ~GYangCZServiceDialog();

private:
    int dim;
    bool isTIF;
    Ui::GYangCZServiceDialog* ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void InputChanged();
    void onCmbObPathChange();
    void onCmbUnknowChange();
    void onBtnBrowseOutputClicked();
    void onXYZChanged();
    void onBtnConfirmClicked();

};

#endif // GYANGCZSERVICEDIALOG_H
