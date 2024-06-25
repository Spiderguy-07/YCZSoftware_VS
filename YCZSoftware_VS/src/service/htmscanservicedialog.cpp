#include "htmscanservicedialog.h"
#include "ui_htmscanservicedialog.h"

HTMSCANServiceDialog::HTMSCANServiceDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HTMSCANServiceDialog)
{
    ui->setupUi(this);

    ui->cmb_choose->setCurrentIndex(0);
    ui->label_l->setVisible(false);
    ui->dsb_l->setVisible(false);
    //ui->label_area->setVisible(false);
    //ui->cmbRangePath->setVisible(false);

    this->initUI(project->layers<QgsVectorLayer*>());
    connect(ui->cmb_choose, &QComboBox::currentTextChanged, this, &HTMSCANServiceDialog::InputChanged);
    connect(ui->btnComfirm, &QPushButton::clicked, this, &HTMSCANServiceDialog::onBtnConfirmClicked);
}

HTMSCANServiceDialog::~HTMSCANServiceDialog()
{
    delete ui;
}

void HTMSCANServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
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
    connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &HTMSCANServiceDialog::onCmbObPathChange);
    connect(ui->btnBrowseNei, &QPushButton::clicked, this, &HTMSCANServiceDialog::onBtnBrowseNeiClicked);
    connect(ui->btnBrowseOutput, &QPushButton::clicked, this, &HTMSCANServiceDialog::onBtnBrowseOutputClicked);
}

void HTMSCANServiceDialog::InputChanged()
{
    int choose = ui->cmb_choose->currentIndex();
    ui->lineOutputPath->clear();
    if (choose == 0)
    {
        ui->lineNeiPath->setVisible(true);
        ui->btnBrowseNei->setVisible(true);
        ui->label_n->setVisible(true);
        ui->label_s->setVisible(true);
        ui->cmb_s->setVisible(true);
        ui->label_l->setVisible(false);
        ui->dsb_l->setVisible(false);
        //ui->label_area->setVisible(false);
        //ui->cmbRangePath->setVisible(false);
    }
    else if (choose == 1)
    {
        ui->lineNeiPath->setVisible(false);
        ui->btnBrowseNei->setVisible(false);
        ui->label_n->setVisible(false);
        ui->label_s->setVisible(false);
        ui->cmb_s->setVisible(false);
        ui->label_l->setVisible(true);
        ui->dsb_l->setVisible(true);
        //ui->label_area->setVisible(true);
        //ui->cmbRangePath->setVisible(true);
    }
    else
    {
        ui->lineNeiPath->setVisible(false);
        ui->btnBrowseNei->setVisible(false);
        ui->label_n->setVisible(false);
        ui->label_s->setVisible(false);
        ui->cmb_s->setVisible(false);
        ui->label_l->setVisible(false);
        ui->dsb_l->setVisible(false);
        //ui->label_area->setVisible(false);
        //ui->cmbRangePath->setVisible(false);
    }
}

void HTMSCANServiceDialog::onCmbObPathChange()
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


    connect(ui->cmb_z, &QComboBox::currentTextChanged, this, &HTMSCANServiceDialog::onXYZChanged);
    connect(ui->cmb_y, &QComboBox::currentTextChanged, this, &HTMSCANServiceDialog::onXYZChanged);
}

void HTMSCANServiceDialog::onBtnBrowseOutputClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("CSV file(*.csv)"));
    ui->lineOutputPath->setText(fileName);
}

void HTMSCANServiceDialog::onBtnBrowseNeiClicked()
{
    // 设置文件对话框的标题
    QString dialogTitle = "Select a CSV or TXT file";

    // 设置文件过滤器，允许选择 CSV 或 TXT 文件
    QString fileFilter = "CSV files (*.csv);;Text files (*.txt)";

    // 打开文件选择对话框，获取选择的文件路径
    QString filePath = QFileDialog::getOpenFileName(this, dialogTitle, QString(), fileFilter);

    ui->lineNeiPath->setText(filePath);
}

void HTMSCANServiceDialog::onXYZChanged()
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

void HTMSCANServiceDialog::onBtnConfirmClicked()
{
    if (ui->lineOutputPath->text().isEmpty()) {
        QMessageBox::critical(nullptr, "Error about output path", "output path is empty!");
        return;
    }
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
    if (ui->cmb_choose->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about output input", "Choose the type of output is not choose!");
        return;
    }
    else if (ui->cmb_choose->currentIndex() == 0)
    {
        if (ui->lineNeiPath->text().isEmpty()) {
            QMessageBox::critical(nullptr, "Error about output input", "Neighbor File is not choose!");
            return;
        }
    }
    QString outputPath = ui->lineOutputPath->text();
    bool is_nei = true;
    QString neiPath = "";
    int s_index = 0;
    double length = 0;
    if (ui->cmb_choose->currentIndex() == 0) {
        is_nei = true;
        s_index = ui->cmb_s->currentIndex();
    }
    else if (ui->cmb_choose->currentIndex() == 1) {
        is_nei = false;
        length = ui->dsb_l->value();
    }
    double Significant = 0;
    int repeat_num = 0;

    Significant = ui->dsb_sig->value();
    repeat_num = ui->sb_mon->value();

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
    this->close();
    emit begin("Amoeba-G");
    emit sendPyParams(obPt_val, obPts_c, length, neiPath, s_index, is_nei, outputPath, dim, Significant, repeat_num);
    emit getOutPath(outputPath, false);

}