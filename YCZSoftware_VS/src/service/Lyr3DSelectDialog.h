#pragma once

#include <QDialog>
#include <json.h>
#include "ui_Lyr3DSelectDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Lyr3DSelectDialogClass; };
QT_END_NAMESPACE

class Lyr3DSelectDialog : public QDialog
{
	Q_OBJECT

public:
	Lyr3DSelectDialog(Json::Value config, QStringList lyrNames, QWidget *parent = nullptr);
	~Lyr3DSelectDialog();

signals:
	void sendSelectedLyrName(QString lyrName);
	void sendSelectedFilePath(QString path);

private:
	Ui::Lyr3DSelectDialogClass* ui;
	Json::Value _mConfig;
	bool isSelectedLyr;
	QStringList lyrNames;

	void initUI(QStringList items);

private slots:
	void onBtnComfirmClicked();
	void onBtnCancelClicked();
	void onBtnBrowseClicked();
	void onCmbPathSelectedItemChanged(const QString& text);
};
