#include "qqplotdialog.h"
#include "ui_qqplotdialog.h"

QQPlotDialog::QQPlotDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QQPlotDialog)
{
    ui->setupUi(this);
    initUI(project->layers<QgsVectorLayer*>());
    connect(ui->bt_draw, &QPushButton::clicked, this, &QQPlotDialog::onBtnDrawClicked);
}

QQPlotDialog::~QQPlotDialog()
{
    delete ui;
}

void QQPlotDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    //ui->sb_number->setValue(5);
    for (const auto& vecLyr : pjLyr) {
        lyrs.append(vecLyr);
        ui->cmb_layer->addItem(vecLyr->name());
    }
    ui->cmb_layer->setCurrentIndex(-1);
    connect(ui->cmb_layer, &QComboBox::currentTextChanged, this, &QQPlotDialog::onCmbLayerChange);
}

void QQPlotDialog::onCmbLayerChange()
{
    ui->cmb_field->clear();
    int index = ui->cmb_layer->currentIndex();
    QgsVectorLayer* lyr = lyrs.at(index);
    const QgsAttributeList attrList = lyr->attributeList();
    QgsAttributeList::const_iterator it = attrList.constBegin();
    for (; it != attrList.constEnd(); it++) {
        //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
        ui->cmb_field->addItem(lyr->attributeDisplayName(*it));
    }
    ui->cmb_field->setCurrentIndex(-1);


}

void QQPlotDialog::onBtnDrawClicked()
{
    int obInd = ui->cmb_layer->currentIndex();
    QgsVectorLayer* obLyr = lyrs.at(obInd);
    QgsFeatureIterator obIter = obLyr->getFeatures();
    QgsFeature obFeat;
    QVector<double> data;

    while (obIter.nextFeature(obFeat)) {
        bool valOk = false;
        double val = obFeat.attribute(ui->cmb_field->currentIndex()).toDouble(&valOk);
        if (!valOk) {
            QString valStr = obFeat.attribute(ui->cmb_field->currentIndex()).toString();
            val = valStr.toDouble(&valOk);
            if (!valOk) {
                QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                return;
            }
        }
        data.append(val);
    }

    ui->QQPlotWidget->setData(data);

}