#include "LoadingXYZDataDialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QUuid>
#include <qgsvectorlayer.h>

LoadingXYZDataDialog::LoadingXYZDataDialog(QString path, QgsProject* project, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::LoadingXYZDataDialogClass())
{
	ui->setupUi(this);
	_mPath = path;
	_mProject = project;
	initUI();
}

LoadingXYZDataDialog::~LoadingXYZDataDialog()
{
	delete ui;
}

void LoadingXYZDataDialog::onBtnComfirmClicked()
{
	createLyrFromXYZ();
}

void LoadingXYZDataDialog::initUI()
{
	ui->linePath->setText(_mPath);
	ui->cmbSeperator->addItems({ SEPERATOR_COMMA_TEXT, SEPERATOR_SPACE_TEXT, SEPERATOR_TAB_TEXT });
	ui->cmbSeperator->setCurrentIndex(-1);

	connect(ui->cmbSeperator, &QComboBox::currentTextChanged, this, &LoadingXYZDataDialog::onCmbSeperatorSelectedItemChanged);
	connect(ui->btnComfirm, &QPushButton::clicked, this, &LoadingXYZDataDialog::onBtnComfirmClicked);
}

void LoadingXYZDataDialog::createLyrFromXYZ()
{
	QString xCol = ui->cmbXField->currentText();
	QString yCol = ui->cmbYField->currentText();
	QString zCol = ui->cmbZField->currentText();
	QFile file(_mPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "Error", "Can not open this file!");
		this->close();
		return;
	}
	QTextStream stream(&file);

	QString lyrUrl = "PointZ?";
	lyrUrl.append(QString("crs=EPSG:4326&"));
	QString attField = "field=id:integer&";
	QString fieldPrefix = "field=";
	int count = 0;
	QgsFields featFields{};
	featFields.append(QgsField("id", QVariant::Int));
	for (const auto& field : _mColNames) {
		featFields.append(QgsField(field, QVariant::String));
		QString tmpField = fieldPrefix + field + ":";
		tmpField.append("string&");
		attField.append(tmpField);
		count++;
	}
	lyrUrl.append(attField);
	lyrUrl.append(QString("index=yes&"));
	lyrUrl.append(QString("memoryid=%1").arg(QUuid::createUuid().toString()));
	QgsVectorLayer::LayerOptions options(_mProject->transformContext());

	QFileInfo fileInfo = QFileInfo(file);
	QgsVectorLayer* tmpVecLyr = new QgsVectorLayer(lyrUrl, fileInfo.fileName(), QString("memory"), options);
	if (!tmpVecLyr->isValid()) {
		QMessageBox::critical(this, "Error occured", "Can not create 3D point layer.");
		this->close();
		return;
	}

	QgsVectorDataProvider* dataProvider = tmpVecLyr->dataProvider();
	tmpVecLyr->startEditing();

	int count2 = 0;
	while (!stream.atEnd()) {
		QString line = stream.readLine();
		if (count2 == 0) {
			count2++;
			continue;
		}
		QStringList attrs = line.split(_mSeperator);
		
		QgsFeature tmpFeature(featFields);
		double x = 0;
		double y = 0;
		double z = 0;
		tmpFeature.setAttribute("id", count2 - 1);
		for (int i = 0; i < attrs.length(); i++) {
			QString attr = attrs.at(i);
			QString col = _mColNames.at(i);
			tmpFeature.setAttribute(col, attr);

			if (col == xCol) {
				bool ok;
				x = attr.toDouble(&ok);
				if (!ok) {
					QMessageBox::critical(this, "Wrong data Type", "Data types of coordinates fields are invaild.");
					delete tmpVecLyr;
					this->close();
					return;
				}
			}
			else if (col == yCol) {
				bool ok;
				y = attr.toDouble(&ok);
				if (!ok) {
					QMessageBox::critical(this, "Wrong data Type", "Data types of coordinates fields are invaild.");
					delete tmpVecLyr;
					this->close();
					return;
				}
			}
			else if (col == zCol) {
				bool ok;
				z = attr.toDouble(&ok);
				if (!ok) {
					QMessageBox::critical(this, "Wrong data Type", "Data types of coordinates fields are invaild.");
					delete tmpVecLyr;
					this->close();
					return;
				}
			}
		}
		QgsPoint pt = QgsPoint(x, y, z);
		tmpFeature.setGeometry(QgsGeometry::fromWkt(pt.asWkt()));
		
		dataProvider->addFeature(tmpFeature);
	}
	tmpVecLyr->commitChanges();
	tmpVecLyr->updateExtents();
	_mProject->addMapLayer(tmpVecLyr);
	file.close();
	this->close();
	return;
}

QStringList LoadingXYZDataDialog::getColNames(QString firstLine, QString seperator)
{
	return firstLine.split(seperator);
}

void LoadingXYZDataDialog::onCmbSeperatorSelectedItemChanged(const QString& text)
{
	ui->cmbXField->clear();
	ui->cmbYField->clear();
	ui->cmbZField->clear();
	QFile file(_mPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "Error", "Can not open this file!");
		this->close();
		return;
	}

	QTextStream stream(&file);
	if (stream.atEnd()) {
		QMessageBox::warning(this, "Empty file", "This file is empty!");
		this->close();
		return;
	}
	
	if (text == SEPERATOR_COMMA_TEXT) {
		_mSeperator = SEPERATOR_COMMA;
	}
	else if (text == SEPERATOR_SPACE_TEXT) {
		_mSeperator = SEPERATOR_SPACE;
	}
	else if (text == SEPERATOR_TAB_TEXT) {
		_mSeperator = SEPERATOR_TAB;
	}

	QString firstLine = stream.readLine();
	QStringList colNames = getColNames(firstLine, _mSeperator);

	_mColNames = colNames;
	for (const auto& colName : colNames) {
		ui->cmbXField->addItem(colName);
		ui->cmbYField->addItem(colName);
		ui->cmbZField->addItem(colName);
	}

	file.close();
}
