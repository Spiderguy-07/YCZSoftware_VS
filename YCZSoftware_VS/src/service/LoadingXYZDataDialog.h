#pragma once

#include <QDialog>
#include <qgsproject.h>
#include "ui_LoadingXYZDataDialog.h"
#include "../../Project.h"

#define SEPERATOR_COMMA_TEXT "Comma(,)"
#define SEPERATOR_SPACE_TEXT "Space( )"
#define SEPERATOR_TAB_TEXT "Tabs(\\t)"
#define SEPERATOR_COMMA ","
#define SEPERATOR_SPACE " "
#define SEPERATOR_TAB "\t"

QT_BEGIN_NAMESPACE
namespace Ui { class LoadingXYZDataDialogClass; };
QT_END_NAMESPACE

class LoadingXYZDataDialog : public QDialog
{
	Q_OBJECT

public:
	LoadingXYZDataDialog(QString path, Project* project, QWidget *parent = nullptr);
	~LoadingXYZDataDialog();

private slots:
	void onCmbSeperatorSelectedItemChanged(const QString& text);
	void onBtnComfirmClicked();
private:
	Ui::LoadingXYZDataDialogClass *ui;

	QString _mPath;
	QString _mSeperator;
	QStringList _mColNames;
	Project* _mProject;

	void initUI();
	void createLyrFromXYZ();
	QStringList getColNames(QString firstLine, QString seperator);
	
};
