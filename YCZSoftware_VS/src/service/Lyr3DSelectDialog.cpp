#include "Lyr3DSelectDialog.h"
#include "FileLoadingService.h"
#include "../macro.h"
#include <QMessageBox>

Lyr3DSelectDialog::Lyr3DSelectDialog(Json::Value config, QStringList lyrNames, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Lyr3DSelectDialogClass())
{

	this->_mConfig = config;
	this->isSelectedLyr = false;
	ui->setupUi(this);
	this->lyrNames = lyrNames;
	initUI(lyrNames);
}

Lyr3DSelectDialog::~Lyr3DSelectDialog()
{
	delete ui;
}

void Lyr3DSelectDialog::onBtnComfirmClicked()
{
	if (isSelectedLyr) {
		emit this->sendSelectedLyrName(ui->cmbPath->currentText());
	}
	else {
		emit this->sendSelectedFilePath(ui->cmbPath->currentText());
	}
	
	this->close();
}

void Lyr3DSelectDialog::initUI(QStringList items)
{
	for (const auto& item : items) {
		ui->cmbPath->addItem(item);
	}
	if (items.length() > 0) {
		this->isSelectedLyr = true;
	}

	this->connect(ui->btnBrowse, &QPushButton::clicked, this, &Lyr3DSelectDialog::onBtnBrowseClicked);
	this->connect(ui->btnComfirm, &QPushButton::clicked, this, &Lyr3DSelectDialog::onBtnComfirmClicked);
	this->connect(ui->btnCancel, &QPushButton::clicked, this, &Lyr3DSelectDialog::onBtnCancelClicked);
	this->connect(ui->cmbPath, &QComboBox::currentTextChanged, this, &Lyr3DSelectDialog::onCmbPathSelectedItemChanged);
}

void Lyr3DSelectDialog::onBtnCancelClicked()
{
	this->close();
}

void Lyr3DSelectDialog::onBtnBrowseClicked()
{
	QStringList lyrFileList = FileLoadingService::open3DFileSelectDialog(this->_mConfig[OPEN_VEC_3D_DIALOG_TITLE].asCString(),
		this->_mConfig[VEC_3D_FILTER_NAME].asCString(), this);
	if (lyrFileList.length() == 0) {
		return;
	}

	QString path = lyrFileList.at(0);
	bool isFound = false;
	for (int i = 0; i < ui->cmbPath->count(); i++)
	{
		if (ui->cmbPath->itemText(i) == path)
		{
			isFound = true;
			ui->cmbPath->setCurrentIndex(i);
			break;
		}
	}
	if (!isFound) {
		ui->cmbPath->addItem(path);
		ui->cmbPath->setCurrentText(path);
		this->isSelectedLyr = false;
	}
	
}

void Lyr3DSelectDialog::onCmbPathSelectedItemChanged(const QString & text)
{
	this->isSelectedLyr = false;
	for (const QString& lyr : this->lyrNames) {
		if (lyr == text) {
			this->isSelectedLyr = true;
			break;
		}
	}

}
