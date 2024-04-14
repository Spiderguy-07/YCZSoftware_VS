#include "logtransdialog.h"
#include "ui_logtransdialog.h"

LogTransDialog::LogTransDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogTransDialog)
{
    ui->setupUi(this);
    initUI(project->layers<QgsVectorLayer*>());
    connect(ui->bt_draw, &QPushButton::clicked, this, &LogTransDialog::onBtnDrawClicked);
}

LogTransDialog::~LogTransDialog()
{
    delete ui;
}

void LogTransDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    //ui->sb_number->setValue(5);
    for (const auto& vecLyr : pjLyr) {
        lyrs.append(vecLyr);
        ui->cmb_layer->addItem(vecLyr->name());
    }
    ui->cmb_layer->setCurrentIndex(-1);
    connect(ui->cmb_layer, &QComboBox::currentTextChanged, this, &LogTransDialog::onCmbLayerChange);
}

void LogTransDialog::onCmbLayerChange()
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

void LogTransDialog::onBtnDrawClicked()
{
    //int obInd = ui->cmb_layer->currentIndex();
    //int layerIndex = ui->cmb_layer->currentIndex();

    int layerIndex = ui->cmb_layer->currentIndex();
    if (layerIndex < 0) {
        QMessageBox::warning(this, "Selection error", "Please select a layer.");
        return;
    }

    QgsVectorLayer* layer = lyrs.at(layerIndex);
    if (!layer) {
        QMessageBox::warning(this, "Layer error", "Failed to get the selected layer.");
        return;
    }

    QString fieldName = ui->cmb_field->currentText();
    if (fieldName.isEmpty()) {
        QMessageBox::warning(this, "Field error", "Please select a field.");
        return;
    }

    QgsFeatureIterator featureIter = layer->getFeatures();
    QgsFeature feature;
    QVector<double> data;
    //QgsVectorLayer* obLyr = lyrs.at(obInd);
    //QgsFeatureIterator obIter = obLyr->getFeatures();
    //QgsFeature obFeat;
    //QVector<double> data;

    layer->startEditing();

    // Check if the log field exists, if not, add it
    QString logFieldName = fieldName + "_log";
    if (layer->fields().indexOf(logFieldName) == -1) {
        QgsField logField(logFieldName, QVariant::Double);
        layer->addAttribute(logField);
        layer->updateFields(); // Update the fields in the layer
    }

    while (featureIter.nextFeature(feature)) {
        bool valOk = false;
        double val = feature.attribute(fieldName).toDouble(&valOk);
        if (!valOk) {
            QString valStr = feature.attribute(fieldName).toString();
            val = valStr.toDouble(&valOk);
            if (!valOk) {
                QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                return;
            }
        }
        data.append(val);
    }
    double minVal = *std::min_element(data.begin(), data.end());

    double offset = 0.0;
    if (minVal <= 0) {
        offset = -minVal + 0.001;
    }

    QgsFeatureIterator featureIter_l = layer->getFeatures();
    QgsFeature feature_l;
    while (featureIter_l.nextFeature(feature_l)) {
        bool valOk = false;
        double val = feature_l.attribute(fieldName).toDouble(&valOk);
        if (!valOk) {
            QString valStr = feature_l.attribute(fieldName).toString();
            val = valStr.toDouble(&valOk);
            if (!valOk) {
                QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                return;
            }
        }
        double logVal = std::log(val + offset);
        feature_l.setAttribute(layer->fields().indexOf(logFieldName), logVal);
        layer->updateFeature(feature_l);
        //layer->updateFeature(feature);
    }
    QMessageBox::information(this, "Success", QString("The offset of the dataset is : %1.").arg(offset));
    this->close();
}
