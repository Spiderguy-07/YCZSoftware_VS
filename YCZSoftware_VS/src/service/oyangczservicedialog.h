#ifndef OYANGCZSERVICEDIALOG_H
#define OYANGCZSERVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include "YangCZPyThread.h"

namespace Ui {
class OYangCZServiceDialog;
}

class OYangCZServiceDialog : public QDialog
{
    Q_OBJECT

signals:
    void sendPyParams(QList<double>observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> un_obpts, double c, int k, int dim, QString outputPath, bool e, bool isTIF);
    void getOutPath(QString,bool);
    void begin(QString);

public:
    explicit OYangCZServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~OYangCZServiceDialog();

private:
    int dim;
    bool isTIF;
    Ui::OYangCZServiceDialog *ui;
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

#endif // OYANGCZSERVICEDIALOG_H
