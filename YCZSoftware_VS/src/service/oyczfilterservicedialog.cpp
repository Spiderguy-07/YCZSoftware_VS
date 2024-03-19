#include "oyczfilterservicedialog.h"
#include <qmessagebox.h>
#include <QFileDialog>

OYCZFilterServiceDialog::OYCZFilterServiceDialog(QgsProject* project, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OYCZFilterServiceDialog)
{
    ui->setupUi(this);
    initUI(project->layers<QgsVectorLayer*>());
}

OYCZFilterServiceDialog::~OYCZFilterServiceDialog()
{
    delete ui;
}


void OYCZFilterServiceDialog::onCmbObPathChange(const QString& path)
{
	ui->cmbObVal->clear();
	int index = ui->cmbObPath->currentIndex();
	QgsVectorLayer* lyr = lyrs.at(index);
	const QgsAttributeList attrList = lyr->attributeList();
	QgsAttributeList::const_iterator it = attrList.constBegin();
	for (; it != attrList.constEnd(); it++) {
		//QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
		ui->cmbObVal->addItem(lyr->attributeDisplayName(*it));
	}
}

void OYCZFilterServiceDialog::onBtnBrowseOutputClicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled.tif", tr("TIF file(*.tif)"));
	ui->lineOutputPath->setText(fileName);
}

void OYCZFilterServiceDialog::onBtnConfirmClicked()
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
	QList<ObPt2D> obPts;

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

		QgsGeometry geo = obFeat.geometry();
		for (auto it = geo.vertices_begin(); it != geo.vertices_end(); it++) {
			QgsPoint coor = QgsPoint(*it);
			ObPt2D pt(coor.x(), coor.y(), val);
			obPts.append(pt);
		}
	}

	QList<Range2D> rangeA;
	int RangInd = ui->cmbRangePath->currentIndex();
	QgsVectorLayer* rangLyr = lyrsR.at(RangInd);
	QgsRectangle layerExtent = rangLyr->extent();
	double minX = layerExtent.xMinimum();
	double maxY = layerExtent.yMaximum();
	double maxX = layerExtent.xMaximum();
	double minY = layerExtent.yMinimum();

	Range2D l_xy(minX, maxY);
	rangeA.append(l_xy);
	Range2D r_xy(maxX, minY);
	rangeA.append(r_xy);

	double sizeA = ui->lineSize->text().toDouble();
	int k_num = ui->sb_k_num->value();
	double c_val = ui->dsb_c_num->value();
	bool error_1 = false;
	if (ui->cb_error->isChecked())
		error_1 = true;

	this->close();
	emit begin("Ordinary YangCZ");
	emit sendPyParams(obPts, rangeA, outputPath, sizeA, k_num, c_val, error_1);
	emit getOutPath(outputPath);
	//emit begin("Ordinary YangCZ");
	
}

void OYCZFilterServiceDialog::onBtnCancelClicked()
{
	this->close();
}


void OYCZFilterServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
	for (const auto& vecLyr : pjLyr) {
		if (vecLyr->wkbType() != Qgis::WkbType::Point &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPoint &&
			vecLyr->wkbType() != Qgis::WkbType::PointM &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPointM &&
			vecLyr->wkbType() != Qgis::WkbType::PointZ &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPointZ &&
			vecLyr->wkbType() != Qgis::WkbType::PointZM &&
			vecLyr->wkbType() != Qgis::WkbType::MultiPointZM) {
			continue;
		}
		lyrs.append(vecLyr);
		ui->cmbObPath->addItem(vecLyr->name());
	}
	for (const auto& vecLyr : pjLyr) {
		lyrsR.append(vecLyr);
		ui->cmbRangePath->addItem(vecLyr->name());
	}
	ui->cmbObPath->setCurrentIndex(-1);
	ui->cmbRangePath->setCurrentIndex(-1);

	connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &OYCZFilterServiceDialog::onCmbObPathChange);
	//connect(ui->cmbUnobPath, &QComboBox::currentTextChanged, this, &YCZFilterServiceDialog::onCmbUnobPathChange);
	connect(ui->btnBrowseOutput, &QPushButton::clicked, this, &OYCZFilterServiceDialog::onBtnBrowseOutputClicked);
	connect(ui->btnComfirm, &QPushButton::clicked, this, &OYCZFilterServiceDialog::onBtnConfirmClicked);
	connect(ui->btnCancel, &QPushButton::clicked, this, &OYCZFilterServiceDialog::onBtnCancelClicked);


}
