#include "YCZFilterServiceDialog.h"

#include <qmessagebox.h>
#include <QFileDialog>



YCZFilterServiceDialog::YCZFilterServiceDialog(QgsProject* project, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::YCZFilterServiceDialogClass())
{
	ui->setupUi(this);
	initUI(project->layers<QgsVectorLayer*>());
}

YCZFilterServiceDialog::~YCZFilterServiceDialog()
{
	delete ui;
}

void YCZFilterServiceDialog::onCmbObPathChange(const QString & path)
{
	ui->cmbObVal->clear();
	ui->cmbObLbl->clear();
	int index = ui->cmbObPath->currentIndex();
	QgsVectorLayer* lyr = lyrs.at(index);
	const QgsAttributeList attrList = lyr->attributeList();
	QgsAttributeList::const_iterator it = attrList.constBegin();
	for (; it != attrList.constEnd(); it++) {
		//QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
		ui->cmbObVal->addItem(lyr->attributeDisplayName(*it));
		ui->cmbObLbl->addItem(lyr->attributeDisplayName(*it));
	}
	
}

void YCZFilterServiceDialog::onCmbUnobPathChange(const QString& path)
{
	ui->cmbUnobLbl->clear();
	int index = ui->cmbUnobPath->currentIndex();
	QgsVectorLayer* lyr = lyrs.at(index);
	const QgsAttributeList attrList = lyr->attributeList();
	QgsAttributeList::const_iterator it = attrList.constBegin();
	for (; it != attrList.constEnd(); it++) {
		ui->cmbUnobLbl->addItem(lyr->attributeDisplayName(*it));
	}
}

void YCZFilterServiceDialog::onBtnBrowseOutputClicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled.csv", tr("csv file(*.csv *.txt)"));
	ui->lineOutputPath->setText(fileName);
}

void YCZFilterServiceDialog::onBtnConfirmClicked()
{
	if (ui->lineOutputPath->text().isEmpty()) {
		QMessageBox::critical(nullptr, "Error about output path", "output path is empty!");
		return;
	}
	QString outputPath = ui->lineOutputPath->text();


	int obInd = ui->cmbObPath->currentIndex();
	QgsVectorLayer* obLyr = lyrs.at(obInd);
	QgsFeatureIterator obIter = obLyr->getFeatures();
	QgsFeature obFeat;
	QList<ObPt> obPts;

	while (obIter.nextFeature(obFeat)) {
		bool valOk = false;
		double val = obFeat.attribute(ui->cmbObVal->currentIndex()).toDouble(&valOk);
		if (!valOk) {
			QString valStr = obFeat.attribute(ui->cmbObVal->currentIndex()).toString();
			val = valStr.toDouble(&valOk);
			if (!valOk) {
				QMessageBox::critical(this, "Illegal data type", "Data type of val should be float.");
				return;
			}
		}

		bool lblOk = false;
		int lbl = obFeat.attribute(ui->cmbObLbl->currentIndex()).toInt(&lblOk);
		if (!lblOk) {
			QString lblStr = obFeat.attribute(ui->cmbObLbl->currentIndex()).toString();
			lbl = lblStr.toInt(&lblOk);
			if (!lblOk) {
				QMessageBox::critical(this, "Illegal data type", "Data type of label should be int.");
				return;
			}
		}

		QgsGeometry geo = obFeat.geometry();
		for (auto it = geo.vertices_begin(); it != geo.vertices_end(); it++) {
			QgsPoint coor = QgsPoint(*it);
			ObPt pt(coor.x(), coor.y(), coor.z(), val, lbl);
			obPts.append(pt);
		}
	}

	QList<UnobPt> unobPts;
	int unobInd = ui->cmbUnobPath->currentIndex();
	QgsVectorLayer* unobLyr = lyrs.at(unobInd);
	QgsFeatureIterator unobIter = unobLyr->getFeatures();
	QgsFeature unobFeat;
	while (unobIter.nextFeature(unobFeat)) {
		bool ok = false;
		int lbl = unobFeat.attribute(ui->cmbUnobLbl->currentIndex()).toInt(&ok);
		if (!ok) {
			QString lblStr = unobFeat.attribute(ui->cmbUnobLbl->currentIndex()).toString();
			lbl = lblStr.toInt(&ok);
			if (!ok) {
				QMessageBox::critical(this, "Illegal data type", "Data type of label should be int.");
				return;
			}
		}

		QgsGeometry geo = unobFeat.geometry();
		for (auto it = geo.vertices_begin(); it != geo.vertices_end(); it++) {
			QgsPoint coor = QgsPoint(*it);
			UnobPt pt(coor.x(), coor.y(), coor.z(), lbl);
			unobPts.append(pt);
		}
	}
	emit sendPyParams(obPts, unobPts, outputPath);

	this->close();
}

void YCZFilterServiceDialog::onBtnCancelClicked()
{
	this->close();
}

void YCZFilterServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
	for (const auto& vecLyr : pjLyr) {
		if (vecLyr->wkbType() != Qgis::WkbType::PointZ &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPointZ &&
			vecLyr->wkbType() != Qgis::WkbType::PointZM &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPointZM) {
			continue;
		}
		lyrs.append(vecLyr);
		ui->cmbObPath->addItem(vecLyr->name());
		ui->cmbUnobPath->addItem(vecLyr->name());
	}
	ui->cmbObPath->setCurrentIndex(-1);
	ui->cmbUnobPath->setCurrentIndex(-1);

	connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &YCZFilterServiceDialog::onCmbObPathChange);
	connect(ui->cmbUnobPath, &QComboBox::currentTextChanged, this, &YCZFilterServiceDialog::onCmbUnobPathChange);
	connect(ui->btnBrowseOutput, &QPushButton::clicked, this, &YCZFilterServiceDialog::onBtnBrowseOutputClicked);
	connect(ui->btnComfirm, &QPushButton::clicked, this, &YCZFilterServiceDialog::onBtnConfirmClicked);
	connect(ui->btnCancel, &QPushButton::clicked, this, &YCZFilterServiceDialog::onBtnCancelClicked);
	
}

