#pragma once

#include <QDialog>
#include <QVector>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include "RunPyService.h"
#include "ui_YCZFilterServiceDialog.h"
#include "YCZFilterPyThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class YCZFilterServiceDialogClass; };
QT_END_NAMESPACE

class YCZFilterServiceDialog : public QDialog
{
	Q_OBJECT
signals:
	void sendPyParams(QList<ObPt> obpts, QList<UnobPt> unobPts, QString outputPath);


public:
	YCZFilterServiceDialog(QgsProject* project, QWidget *parent = nullptr);
	~YCZFilterServiceDialog();


private slots:
	void onCmbObPathChange(const QString& path);
	void onCmbUnobPathChange(const QString& path);
	void onBtnBrowseOutputClicked();
	void onBtnConfirmClicked();
	void onBtnCancelClicked();
	//void onPyThreadFinished();

private:

	Ui::YCZFilterServiceDialogClass *ui;
	QVector<QgsVectorLayer*> lyrs;
	void initUI(QVector<QgsVectorLayer*> pjLyr);
};
