#pragma once

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include "ui_oyczfilterservicedialog.h"
#include "YCZFilterPyThread2D.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OYCZFilterServiceDialogClass; };
QT_END_NAMESPACE

class OYCZFilterServiceDialog : public QDialog
{
    Q_OBJECT
signals:
    void sendPyParams(QList<ObPt2D> obpts, QList<Range2D> rangeA, QString outputPath, double s, int k,double c, bool e);
	void getOutPath(QString);
	void begin(QString);


public:
    explicit OYCZFilterServiceDialog(QgsProject* project, QWidget *parent = nullptr);
    ~OYCZFilterServiceDialog();

	//QString getOutPath();

private slots:
	void onCmbObPathChange(const QString& path);
	//void onCmbRangePathChange(const QString& path);
	void onBtnBrowseOutputClicked();
	void onBtnConfirmClicked();
	void onBtnCancelClicked();
	
	//void onPyThreadFinished();

private:

	Ui::OYCZFilterServiceDialog* ui;
	QVector<QgsVectorLayer*> lyrs;
	QVector<QgsVectorLayer*> lyrsR;
	void initUI(QVector<QgsVectorLayer*> pjLyr);
};

