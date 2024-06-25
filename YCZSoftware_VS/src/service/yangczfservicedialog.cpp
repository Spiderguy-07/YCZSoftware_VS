#include "yangczfservicedialog.h"
#include "ui_yangczfservicedialog.h"

YangCZFServiceDialog::YangCZFServiceDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YangCZFServiceDialog)
{
    ui->setupUi(this);

    this->initUI(project->layers<QgsVectorLayer*>());

    connect(ui->btnComfirm, &QPushButton::clicked, this, &YangCZFServiceDialog::onBtnConfirmClicked);
}

YangCZFServiceDialog::~YangCZFServiceDialog()
{
    delete ui;
}

void YangCZFServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    dim = 0;
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
    ui->cmbObPath->setCurrentIndex(-1);
    connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &YangCZFServiceDialog::onCmbObPathChange);
}

void YangCZFServiceDialog::onCmbObPathChange()
{
    ui->cmbObVal->clear();
    ui->cmb_x->clear();
    ui->cmb_y->clear();
    ui->cmb_z->clear();
    int index = ui->cmbObPath->currentIndex();
    //ui->cmbRangePath->setCurrentIndex(index);
    QgsVectorLayer* lyr = lyrs.at(index);
    const QgsAttributeList attrList = lyr->attributeList();
    QgsAttributeList::const_iterator it = attrList.constBegin();
    ui->cmb_y->addItem("<None>");
    ui->cmb_z->addItem("<None>");
    for (; it != attrList.constEnd(); it++) {
        //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
        ui->cmbObVal->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_x->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_y->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_z->addItem(lyr->attributeDisplayName(*it));
    }
    ui->cmbObVal->setCurrentIndex(-1);
    ui->cmb_x->setCurrentIndex(0);
    ui->cmb_y->setCurrentIndex(0);
    ui->cmb_z->setCurrentIndex(0);

    ui->cmb_y->setEnabled(true);
    ui->cmb_z->setEnabled(false);


    connect(ui->cmb_z, &QComboBox::currentTextChanged, this, &YangCZFServiceDialog::onXYZChanged);
    connect(ui->cmb_y, &QComboBox::currentTextChanged, this, &YangCZFServiceDialog::onXYZChanged);
}

void YangCZFServiceDialog::onXYZChanged()
{
    if (ui->cmb_y->currentIndex() > 0)
    {
        ui->cmb_z->setEnabled(true);
    }
    else
    {
        ui->cmb_z->setCurrentIndex(0);
        ui->cmb_z->setEnabled(false);
    }
}

void YangCZFServiceDialog::onBtnConfirmClicked()
{
    if (ui->cmbObPath->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about output input", "Observed Data Layer is not choose!");
        return;
    }
    if (ui->cmbObVal->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about output input", "Observed Data Field is not choose!");
        return;
    }

    int k_num = ui->sb_k_num->value();
    double c_val = ui->dsb_c_num->value();

    QList<ObPtXYZ> obPts_c;
    QList<double> obPt_val;

    int obInd = ui->cmbObPath->currentIndex();
    QgsVectorLayer* obLyr = lyrs.at(obInd);
    QgsFeatureIterator obIter = obLyr->getFeatures();
    QgsFeature obFeat;


    if (ui->cmb_y->currentIndex() == 0) {
        dim = 1;
        while (obIter.nextFeature(obFeat)) {
            bool valOk = false;
            double val = obFeat.attribute(ui->cmbObVal->currentIndex()).toDouble(&valOk);
            double X = obFeat.attribute(ui->cmb_x->currentIndex()).toDouble(&valOk);
            double Y = 0;
            double Z = 0;

            if (!valOk) {
                QString valStr = obFeat.attribute(ui->cmbObVal->currentIndex()).toString();
                QString XStr = obFeat.attribute(ui->cmb_x->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                X = XStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            ObPtXYZ xyz(X, Y, Z);
            obPts_c.append(xyz);
            obPt_val.append(val);
        }
    }
    else if (ui->cmb_y->currentIndex() > 0 && ui->cmb_z->currentIndex() == 0) {
        dim = 2;
        while (obIter.nextFeature(obFeat)) {
            bool valOk = false;
            double val = obFeat.attribute(ui->cmbObVal->currentIndex()).toDouble(&valOk);
            double X = obFeat.attribute(ui->cmb_x->currentIndex()).toDouble(&valOk);
            double Y = obFeat.attribute(ui->cmb_y->currentIndex() - 1).toDouble(&valOk);
            double Z = 0;

            if (!valOk) {
                QString valStr = obFeat.attribute(ui->cmbObVal->currentIndex()).toString();
                QString XStr = obFeat.attribute(ui->cmb_x->currentIndex()).toString();
                QString YStr = obFeat.attribute(ui->cmb_y->currentIndex() - 1).toString();
                val = valStr.toDouble(&valOk);
                X = XStr.toDouble(&valOk);
                Y = YStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            ObPtXYZ xyz(X, Y, Z);
            obPts_c.append(xyz);
            obPt_val.append(val);
        }

    }
    else {
        dim = 3;
        while (obIter.nextFeature(obFeat)) {
            bool valOk = false;
            double val = obFeat.attribute(ui->cmbObVal->currentIndex()).toDouble(&valOk);
            double X = obFeat.attribute(ui->cmb_x->currentIndex()).toDouble(&valOk);
            double Y = obFeat.attribute(ui->cmb_y->currentIndex() - 1).toDouble(&valOk);
            double Z = obFeat.attribute(ui->cmb_z->currentIndex() - 1).toDouble(&valOk);

            if (!valOk) {
                QString valStr = obFeat.attribute(ui->cmbObVal->currentIndex()).toString();
                QString XStr = obFeat.attribute(ui->cmb_x->currentIndex()).toString();
                QString YStr = obFeat.attribute(ui->cmb_y->currentIndex() - 1).toString();
                QString ZStr = obFeat.attribute(ui->cmb_z->currentIndex() - 1).toString();
                val = valStr.toDouble(&valOk);
                X = XStr.toDouble(&valOk);
                Y = YStr.toDouble(&valOk);
                Z = ZStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            ObPtXYZ xyz(X, Y, Z);
            obPts_c.append(xyz);
            obPt_val.append(val);
        }
    }

    //int obInd_r = ui->cmbRangePath->currentIndex();
    //QgsVectorLayer* obLyr_r = lyrs.at(obInd_r);
    //QgsRectangle layerExtent = obLyr->extent();

    // Check for duplicate points in obPts_c and remove duplicates
    bool break_show = false;
    bool run_code = true;
    for (int i = 0; i < obPts_c.size(); ++i) {
        for (int j = i + 1; j < obPts_c.size(); ++j) {
            if (obPts_c[i].x == obPts_c[j].x && obPts_c[i].y == obPts_c[j].y && obPts_c[i].z == obPts_c[j].z) {
                //run_code = false;
                // Remove the duplicate point and its corresponding value
                switch (QMessageBox::question(this, QString(), tr("There are coincident points in the provided coordinate fields"), tr("  Take the average  "), tr("  Leave first point  "), tr("Cancel"))) {
                case QMessageBox::Yes:
                    run_code = true;
                    // Take the average action
                    for (int i = 0; i < obPts_c.size(); ++i) {
                        double sum_opt = obPt_val[i];
                        int count_opt = 1;
                        bool check_opt = false;
                        for (int j = i + 1; j < obPts_c.size(); ++j) {
                            if (obPts_c[i].x == obPts_c[j].x && obPts_c[i].y == obPts_c[j].y && obPts_c[i].z == obPts_c[j].z) {
                                // Remove the duplicate point and its corresponding value
                                check_opt = true;
                                sum_opt = sum_opt + obPt_val[j];
                                count_opt++;
                                obPts_c.removeAt(j);
                                obPt_val.removeAt(j);
                                // Adjust the loop index since we removed an element
                                --j;
                            }
                        }
                        if (check_opt) {
                            obPt_val[i] = sum_opt / count_opt;
                        }
                    }
                    break;
                case QMessageBox::No:     
                    run_code = true;
                    // Leave the first point in place action
                    for (int i = 0; i < obPts_c.size(); ++i) {
                        for (int j = i + 1; j < obPts_c.size(); ++j) {
                            if (obPts_c[i].x == obPts_c[j].x && obPts_c[i].y == obPts_c[j].y && obPts_c[i].z == obPts_c[j].z) {
                                // Remove the duplicate point and its corresponding value
                                obPts_c.removeAt(j);
                                obPt_val.removeAt(j);
                                // Adjust the loop index since we removed an element
                                --j;
                            }
                        }
                    }
                    break;
                
                case QMessageBox::Cancel:
                    run_code = false;
                    break;
                }
                break_show = true;
                break;
            }
            if (break_show)
                break;
        }
    }
    if (run_code) {
        this->close();
        emit begin("YangCZ Filter");
        emit sendPyParams(obPt_val, obPts_c, c_val, k_num, dim);
    }
}

