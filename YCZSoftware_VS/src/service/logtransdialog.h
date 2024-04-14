#ifndef LOGTRANSDIALOG_H
#define LOGTRANSDIALOG_H

#include <QDialog>
#include <QVector>
#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qmessagebox.h>

namespace Ui {
class LogTransDialog;
}

class LogTransDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogTransDialog(QgsProject* project, QWidget *parent = nullptr);
    ~LogTransDialog();

private:
    Ui::LogTransDialog *ui;
    QVector<QgsVectorLayer*> lyrs;
    void initUI(QVector<QgsVectorLayer*> pjLyr);

private slots:
    void onCmbLayerChange();
    void onBtnDrawClicked();
};

#endif // LOGTRANSDIALOG_H
