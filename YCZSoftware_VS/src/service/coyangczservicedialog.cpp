#include "coyangczservicedialog.h"
#include "ui_coyangczservicedialog.h"

CoYangCZServiceDialog::CoYangCZServiceDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoYangCZServiceDialog)
{
    ui->setupUi(this);
    this->initUI(project->layers<QgsVectorLayer*>());
    connect(ui->cmb_choose, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::InputChanged);
    connect(ui->btnComfirm, &QPushButton::clicked, this, &CoYangCZServiceDialog::onBtnConfirmClicked);
}

CoYangCZServiceDialog::~CoYangCZServiceDialog()
{
    delete ui;
}

void CoYangCZServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    dim = 0;
    isTIF = false;
    ui->cmb_l2->addItem("None");
    ui->cmb_l3->addItem("None");
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
        ui->cmb_unknow->addItem(vecLyr->name());
        ui->cmb_l1->addItem(vecLyr->name());
        ui->cmb_l2->addItem(vecLyr->name());
        ui->cmb_l3->addItem(vecLyr->name());
    }
    ui->cmbObPath->setCurrentIndex(-1);
    ui->cmb_unknow->setCurrentIndex(-1);
    ui->cmb_l1->setCurrentIndex(-1);
    ui->cmb_l2->setCurrentIndex(-1);
    ui->cmb_l3->setCurrentIndex(-1);
    ui->cmb_l2->setEnabled(false);
    ui->cmb_l3->setEnabled(false);
    ui->cmb_f2->setEnabled(false);
    ui->cmb_f3->setEnabled(false);
    ui->dsb_c3->setEnabled(false);
    ui->dsb_c4->setEnabled(false);

    connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onCmbObPathChange);
    connect(ui->cmb_unknow, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onCmbUnknowChange);
    connect(ui->cmb_l1, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onCmbCo1Change);
    connect(ui->cmb_l2, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onCmbCo2Change);
    connect(ui->cmb_l3, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onCmbCo3Change);
    connect(ui->btnBrowseOutput, &QPushButton::clicked, this, &CoYangCZServiceDialog::onBtnBrowseOutputClicked);
}


void CoYangCZServiceDialog::InputChanged()
{
    int choose = ui->cmb_choose->currentIndex();
    ui->lineOutputPath->clear();
    if (choose == 0)
    {
        ui->label_unknow->setVisible(true);
        ui->cmb_unknow->setVisible(true);
        ui->label_unknow_c->setVisible(true);
        ui->xyz_widget->setVisible(true);
        ui->label_grid->setVisible(false);
        ui->line_size->setVisible(false);
        //ui->label_area->setVisible(false);
        //ui->cmbRangePath->setVisible(false);
    }
    else if (choose == 1)
    {
        ui->label_grid->setVisible(true);
        ui->line_size->setVisible(true);
        ui->label_unknow->setVisible(false);
        ui->cmb_unknow->setVisible(false);
        ui->label_unknow_c->setVisible(false);
        ui->xyz_widget->setVisible(false);
        //ui->label_area->setVisible(true);
        //ui->cmbRangePath->setVisible(true);
    }
    else
    {
        ui->label_unknow->setVisible(false);
        ui->cmb_unknow->setVisible(false);
        ui->label_unknow_c->setVisible(false);
        ui->label_grid->setVisible(false);
        ui->line_size->setVisible(false);
        ui->xyz_widget->setVisible(false);
        //ui->label_area->setVisible(false);
        //ui->cmbRangePath->setVisible(false);
    }
}


void CoYangCZServiceDialog::onCmbObPathChange()
{
    ui->cmbObVal->clear();
    ui->cmb_x->clear();
    ui->cmb_y->clear();
    ui->cmb_z->clear();
    int index = ui->cmbObPath->currentIndex();
    ui->cmb_unknow->setCurrentIndex(index);
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
    ui->cmb_z_u->setEnabled(false);
    ui->cmb_z_co->setEnabled(false);
    ui->cmb_y_u->setCurrentIndex(-1);
    ui->cmb_z_u->setCurrentIndex(-1);

    connect(ui->cmb_z, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onXYZChanged);
    connect(ui->cmb_y, &QComboBox::currentTextChanged, this, &CoYangCZServiceDialog::onXYZChanged);
}

void CoYangCZServiceDialog::onCmbUnknowChange()
{
    ui->cmb_x_u->clear();
    ui->cmb_y_u->clear();
    ui->cmb_z_u->clear();
    int index = ui->cmb_unknow->currentIndex();
    QgsVectorLayer* lyr = lyrs.at(index);
    const QgsAttributeList attrList = lyr->attributeList();
    QgsAttributeList::const_iterator it = attrList.constBegin();
    for (; it != attrList.constEnd(); it++) {
        //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
        ui->cmb_x_u->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_y_u->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_z_u->addItem(lyr->attributeDisplayName(*it));
    }
    ui->cmb_x_u->setCurrentIndex(-1);
    ui->cmb_y_u->setCurrentIndex(-1);
    ui->cmb_z_u->setCurrentIndex(-1);
}

void CoYangCZServiceDialog::onCmbCo1Change()
{
    if (ui->cmb_l1->currentIndex() >= 0) {
        ui->cmb_l2->setEnabled(true);
        ui->cmb_f2->setEnabled(true);
        //ui->dsb_c3->setEnabled(true);
        ui->cmb_f1->clear();
        int index = ui->cmb_l1->currentIndex();
        QgsVectorLayer* lyr = lyrs.at(index);
        const QgsAttributeList attrList = lyr->attributeList();
        QgsAttributeList::const_iterator it = attrList.constBegin();
        for (; it != attrList.constEnd(); it++) {
            //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
            ui->cmb_f1->addItem(lyr->attributeDisplayName(*it));
            ui->cmb_x_co->addItem(lyr->attributeDisplayName(*it));
            ui->cmb_y_co->addItem(lyr->attributeDisplayName(*it));
            ui->cmb_z_co->addItem(lyr->attributeDisplayName(*it));
        }
        ui->cmb_f1->setCurrentIndex(-1);
        ui->cmb_x_co->setCurrentIndex(-1);
        ui->cmb_y_co->setCurrentIndex(-1);
        ui->cmb_z_co->setCurrentIndex(-1);
    }
    else {
        ui->cmb_l2->clear();
        ui->cmb_l3->clear();
        ui->cmb_f1->clear();
        ui->cmb_f2->clear();
        ui->cmb_f3->clear();
        ui->cmb_l2->setEnabled(false);
        ui->cmb_f2->setEnabled(false);
        ui->dsb_c3->setValue(0);
        ui->dsb_c4->setValue(0);
        ui->dsb_c3->setEnabled(false);
        ui->dsb_c4->setEnabled(false);
    }
}

void CoYangCZServiceDialog::onCmbCo2Change()
{
    if (ui->cmb_l2->currentIndex() > 0) {
        ui->cmb_f2->clear();
        ui->dsb_c3->setEnabled(true);
        ui->cmb_l3->setEnabled(true);
        ui->cmb_f3->setEnabled(true);
        int index = ui->cmb_l2->currentIndex() - 1;
        QgsVectorLayer* lyr = lyrs.at(index);
        const QgsAttributeList attrList = lyr->attributeList();
        QgsAttributeList::const_iterator it = attrList.constBegin();
        for (; it != attrList.constEnd(); it++) {
            //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
            ui->cmb_f2->addItem(lyr->attributeDisplayName(*it));
        }
        ui->cmb_f2->setCurrentIndex(-1);
    }
    else {
        ui->cmb_l3->clear();
        ui->cmb_f2->clear();
        ui->cmb_f3->clear();
        ui->cmb_l3->setEnabled(false);
        ui->cmb_f3->setEnabled(false);
        ui->dsb_c3->setValue(0);
        ui->dsb_c4->setValue(0);
        ui->dsb_c3->setEnabled(false);
        ui->dsb_c4->setEnabled(false);
    }
}

void CoYangCZServiceDialog::onCmbCo3Change()
{
    if (ui->cmb_l3->currentIndex() > 0) {
        ui->cmb_f3->clear();
        ui->dsb_c4->setEnabled(true);
        int index = ui->cmb_l2->currentIndex() - 1;
        QgsVectorLayer* lyr = lyrs.at(index);
        const QgsAttributeList attrList = lyr->attributeList();
        QgsAttributeList::const_iterator it = attrList.constBegin();
        for (; it != attrList.constEnd(); it++) {
            //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
            ui->cmb_f3->addItem(lyr->attributeDisplayName(*it));
        }
        ui->cmb_f3->setCurrentIndex(-1);
    }
    else {
        ui->cmb_f3->clear();
        ui->dsb_c4->setValue(0);
        ui->dsb_c4->setEnabled(false);
    }
}

void CoYangCZServiceDialog::onXYZChanged()
{
    if (ui->cmb_y->currentIndex() > 0)
    {
        ui->cmb_z->setEnabled(true);

        ui->cmb_y_u->setEnabled(true);
        if (ui->cmb_z->currentIndex() > 0)
        {
            ui->cmb_z_u->setEnabled(true);
            ui->cmb_z_co->setEnabled(true);
            ui->cmb_choose->setCurrentIndex(0);
            ui->cmb_choose->setEnabled(false);
        }
        else
        {
            ui->cmb_z_u->setCurrentIndex(-1);
            ui->cmb_z_u->setEnabled(false);
            ui->cmb_z_co->setCurrentIndex(-1);
            ui->cmb_z_co->setEnabled(false);
            ui->cmb_choose->setEnabled(true);
        }
    }
    else
    {
        ui->cmb_y_u->setCurrentIndex(-1);
        ui->cmb_z_u->setCurrentIndex(-1);
        ui->cmb_z->setCurrentIndex(0);
        ui->cmb_z->setEnabled(false);
        ui->cmb_z_u->setEnabled(false);
        ui->cmb_y_u->setEnabled(false);
        ui->cmb_choose->setEnabled(true);
        ui->cmb_z_co->setCurrentIndex(-1);
        ui->cmb_z_co->setEnabled(false);
        ui->cmb_y_co->setCurrentIndex(-1);
        ui->cmb_y_co->setEnabled(false);
    }
}

void CoYangCZServiceDialog::onBtnBrowseOutputClicked()
{
    int choose = ui->cmb_choose->currentIndex();
    if (choose == 1)
    {
        if (ui->cmb_y->currentIndex() > 0)
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("TIF file(*.tif)"));
            ui->lineOutputPath->setText(fileName);
        }
        else if (ui->cmbObPath->currentIndex() != -1)
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("CSV file(*.csv)"));
            ui->lineOutputPath->setText(fileName);
        }
        else
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("Unknow file(*.)"));
            ui->lineOutputPath->setText(fileName);
        }

    }
    else if (choose == 0)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("CSV file(*.csv)"));
        ui->lineOutputPath->setText(fileName);
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("Unknow file(*.)"));
        ui->lineOutputPath->setText(fileName);
    }
}

void CoYangCZServiceDialog::onBtnConfirmClicked()
{
    if (ui->lineOutputPath->text().isEmpty()) {
        QMessageBox::critical(nullptr, "Error about output path", "output path is empty!");
        return;
    }
    if (ui->cmbObPath->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about input", "Observed Data Layer is not choose!");
        return;
    }
    if (ui->cmbObVal->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about input", "Observed Data Field is not choose!");
        return;
    }
    if (ui->cmb_choose->currentIndex() == -1)
    {
        QMessageBox::critical(nullptr, "Error about input", "Choose the type of output is not choose!");
        return;
    }
    else if (ui->cmb_choose->currentIndex() == 0)
    {
        if (ui->cmb_unknow->currentIndex() == -1) {
            QMessageBox::critical(nullptr, "Error about input", "Unknow Data Layer is not choose!");
            return;
        }
    }
    else if (ui->cmb_choose->currentIndex() == 1)
    {
        if (ui->line_size->text().isEmpty()) {
            QMessageBox::critical(nullptr, "Error about output input", "The interval size is empty!");
            return;
        }
    }
    if (ui->cmb_l1->currentIndex() == -1 || ui->cmb_f2->currentIndex() == 0) {
        QMessageBox::critical(nullptr, "Error about input", "There are no covariates!");
        return;
    }

    QString outputPath = ui->lineOutputPath->text();
    int k_num = ui->sb_k_num->value();
    QList<double> c_list;
 
    bool is_nug = false;
    if (ui->cb_error->isChecked())
        is_nug = true;

    int num_co;

    QList<ObPtXYZ> obPts_c;
    QList<ObPtXYZ> un_obPts_c;
    QList<ObPtXYZ> co_obPts_c;
    QList<double> obPt_val;
    QList<ObPtXYZ> co_obPt_val;

    int obInd = ui->cmbObPath->currentIndex();
    QgsVectorLayer* obLyr = lyrs.at(obInd);
    QgsFeatureIterator obIter = obLyr->getFeatures();
    QgsFeature obFeat;

    int obInd_u = ui->cmb_unknow->currentIndex();
    QgsVectorLayer* obLyr_u = lyrs.at(obInd_u);
    QgsFeatureIterator obIter_u = obLyr_u->getFeatures();
    QgsFeature obFeat_u;

    int obInd_co = ui->cmb_l1->currentIndex();
    QgsVectorLayer* obLyr_co = lyrs.at(obInd_co);
    QgsFeatureIterator obIter_co = obLyr_co->getFeatures();
    QgsFeature obFeat_co;
    //int obInd_r = ui->cmbRangePath->currentIndex();
    //QgsVectorLayer* obLyr_r = lyrs.at(obInd_r);
    //QgsRectangle layerExtent = obLyr->extent();

    c_list.append(ui->dsb_c1->value());
    c_list.append(ui->dsb_c2->value());
    c_list.append(ui->dsb_c3->value());
    c_list.append(ui->dsb_c4->value());

    if (ui->cmb_l3->currentIndex() > 0) {
        num_co = 3;
        int obInd_co1 = ui->cmb_l1->currentIndex();
        QgsVectorLayer* obLyr_co1 = lyrs.at(obInd_co1);
        QgsFeatureIterator obIter_co1 = obLyr_co1->getFeatures();
        QgsFeature obFeat_co1;

        int obInd_co2 = ui->cmb_l2->currentIndex() - 1;
        QgsVectorLayer* obLyr_co2 = lyrs.at(obInd_co2);
        QgsFeatureIterator obIter_co2 = obLyr_co2->getFeatures();
        QgsFeature obFeat_co2;

        int obInd_co3 = ui->cmb_l3->currentIndex() - 1;
        QgsVectorLayer* obLyr_co3 = lyrs.at(obInd_co3);
        QgsFeatureIterator obIter_co3 = obLyr_co3->getFeatures();
        QgsFeature obFeat_co3;

        QList<double> Ob_co1;
        QList<double> Ob_co2;
        QList<double> Ob_co3;

        while (obIter_co1.nextFeature(obFeat_co1)) {
            bool valOk = false;
            double val = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co1.append(val);
        }
        while (obIter_co2.nextFeature(obFeat_co2)) {
            bool valOk = false;
            double val = obFeat_co2.attribute(ui->cmb_f2->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat_co2.attribute(ui->cmb_f2->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co2.append(val);
        }
        while (obIter_co3.nextFeature(obFeat_co3)) {
            bool valOk = false;
            double val = obFeat.attribute(ui->cmb_f3->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat.attribute(ui->cmb_f3->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co3.append(val);
        }

        if (Ob_co1.size() == Ob_co2.size() == Ob_co3.size()) {
            for (int i = 0; i < Ob_co1.size(); i++) {
                ObPtXYZ xyz(Ob_co1[i], Ob_co2[i], Ob_co3[i]);
                co_obPt_val.append(xyz);
            }
        }
        else {
            QMessageBox::critical(this, "Input error", "The size of covariates is not same.");
            return;
        }
    }

    else if (ui->cmb_l2->currentIndex() > 0) {
        num_co = 2;
        int obInd_co1 = ui->cmb_l1->currentIndex();
        QgsVectorLayer* obLyr_co1 = lyrs.at(obInd_co1);
        QgsFeatureIterator obIter_co1 = obLyr_co1->getFeatures();
        QgsFeature obFeat_co1;

        int obInd_co2 = ui->cmb_l2->currentIndex() - 1;
        QgsVectorLayer* obLyr_co2 = lyrs.at(obInd_co2);
        QgsFeatureIterator obIter_co2 = obLyr_co2->getFeatures();
        QgsFeature obFeat_co2;

        QList<double> Ob_co1;
        QList<double> Ob_co2;

        while (obIter_co1.nextFeature(obFeat_co1)) {
            bool valOk = false;
            double val = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co1.append(val);
        }
        while (obIter_co2.nextFeature(obFeat_co2)) {
            bool valOk = false;
            double val = obFeat_co2.attribute(ui->cmb_f2->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat_co2.attribute(ui->cmb_f2->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co2.append(val);
        }
        std::cout << Ob_co1.size();
        std::cout << Ob_co2.size();


        if (Ob_co1.size() == Ob_co2.size()) {
            for (int i = 0; i < Ob_co1.size(); i++) {
                ObPtXYZ xyz(Ob_co1[i], Ob_co2[i], 0);
                co_obPt_val.append(xyz);
            }
        }
        else {
            QMessageBox::critical(this, "Input error", "The size of covariates is not same.");
            return;
        }
    }

    else {
        num_co = 1;
        int obInd_co1 = ui->cmb_l1->currentIndex();
        QgsVectorLayer* obLyr_co1 = lyrs.at(obInd_co1);
        QgsFeatureIterator obIter_co1 = obLyr_co1->getFeatures();
        QgsFeature obFeat_co1;

        QList<double> Ob_co1;

        while (obIter_co1.nextFeature(obFeat_co1)) {
            bool valOk = false;
            double val = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toDouble(&valOk);
            if (!valOk) {
                QString valStr = obFeat_co1.attribute(ui->cmb_f1->currentIndex()).toString();
                val = valStr.toDouble(&valOk);
                if (!valOk) {
                    QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                    return;
                }
            }

            Ob_co1.append(val);
        }

        for (int i = 0; i < Ob_co1.size(); i++) {
            ObPtXYZ xyz(Ob_co1[i], 0, 0);
            co_obPt_val.append(xyz);
        }
    }


    if (ui->cmb_choose->currentIndex() == 0) {
        outputPath = outputPath + ".csv";
        isTIF = false;

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

            while (obIter_u.nextFeature(obFeat_u)) {
                bool valOk = false;
                double X_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toDouble(&valOk);
                double Y_u = 0;
                double Z_u = 0;
                if (!valOk) {
                    QString XStr_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toString();
                    X_u = XStr_u.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_u(X_u, Y_u, Z_u);
                un_obPts_c.append(xyz_u);
            }

            while (obIter_co.nextFeature(obFeat_co)) {
                bool valOk = false;
                double X_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toDouble(&valOk);
                double Y_co = 0;
                double Z_co = 0;
                if (!valOk) {
                    QString XStr_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toString();
                    X_co = XStr_co.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_co(X_co, Y_co, Z_co);
                co_obPts_c.append(xyz_co);
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

            while (obIter_u.nextFeature(obFeat_u)) {
                bool valOk = false;
                double X_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toDouble(&valOk);
                double Y_u = obFeat_u.attribute(ui->cmb_y_u->currentIndex()).toDouble(&valOk);
                double Z_u = 0;
                if (!valOk) {
                    QString XStr_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toString();
                    QString YStr_u = obFeat_u.attribute(ui->cmb_y_u->currentIndex()).toString();

                    X_u = XStr_u.toDouble(&valOk);
                    Y_u = YStr_u.toDouble(&valOk);
                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_u(X_u, Y_u, Z_u);
                un_obPts_c.append(xyz_u);
            }

            while (obIter_co.nextFeature(obFeat_co)) {
                bool valOk = false;
                double X_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toDouble(&valOk);
                double Y_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toDouble(&valOk);
                double Z_co = 0;
                if (!valOk) {
                    QString XStr_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toString();
                    QString YStr_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toString();

                    X_co = XStr_co.toDouble(&valOk);
                    Y_co = YStr_co.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_co(X_co, Y_co, Z_co);
                co_obPts_c.append(xyz_co);
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

            while (obIter_u.nextFeature(obFeat_u)) {
                bool valOk = false;
                double X_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toDouble(&valOk);
                double Y_u = obFeat_u.attribute(ui->cmb_y_u->currentIndex()).toDouble(&valOk);
                double Z_u = obFeat_u.attribute(ui->cmb_z_u->currentIndex()).toDouble(&valOk);
                if (!valOk) {
                    QString XStr_u = obFeat_u.attribute(ui->cmb_x_u->currentIndex()).toString();
                    QString YStr_u = obFeat_u.attribute(ui->cmb_y_u->currentIndex()).toString();
                    QString ZStr_u = obFeat_u.attribute(ui->cmb_z_u->currentIndex()).toString();

                    X_u = XStr_u.toDouble(&valOk);
                    Y_u = YStr_u.toDouble(&valOk);
                    Z_u = ZStr_u.toDouble(&valOk);
                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_u(X_u, Y_u, Z_u);
                un_obPts_c.append(xyz_u);
            }

            while (obIter_co.nextFeature(obFeat_co)) {
                bool valOk = false;
                double X_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toDouble(&valOk);
                double Y_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toDouble(&valOk);
                double Z_co = obFeat_co.attribute(ui->cmb_z_co->currentIndex()).toDouble(&valOk);
                if (!valOk) {
                    QString XStr_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toString();
                    QString YStr_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toString();
                    QString ZStr_co = obFeat_co.attribute(ui->cmb_z_co->currentIndex()).toString();

                    X_co = XStr_co.toDouble(&valOk);
                    Y_co = YStr_co.toDouble(&valOk);
                    Z_co = ZStr_co.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_co(X_co, Y_co, Z_co);
                co_obPts_c.append(xyz_co);
            }
        }
    }
    else
    {
        double size = ui->line_size->text().toDouble();

        if (ui->cmb_y->currentIndex() == 0) {
            isTIF = false;
            dim = 1;
            outputPath = outputPath + ".csv";
            double minX = DBL_MAX;

            double maxX = -DBL_MAX;
            while (obIter_co.nextFeature(obFeat_co)) {
                bool valOk = false;
                double X_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toDouble(&valOk);
                double Y_co = 0;
                double Z_co = 0;
                if (!valOk) {
                    QString XStr_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toString();
                    X_co = XStr_co.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_co(X_co, Y_co, Z_co);
                co_obPts_c.append(xyz_co);
            }

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

                if (X < minX)minX = X;
                if (X > maxX)maxX = X;


                ObPtXYZ xyz(X, Y, Z);
                obPts_c.append(xyz);
                obPt_val.append(val);
            }


            double length = maxX - minX;

            int n_x = length / size + 2;


            if (n_x < 2 || n_x>10000) {
                QMessageBox::critical(nullptr, "Error about input grid size", "Input grid size is too small or too big!");
                return;
            }

            for (int i_x = 0; i_x < n_x; i_x++) {
                double X_r = minX + size * i_x;
                double Y_r = 0;
                double Z_r = 0;

                ObPtXYZ xyz_r(X_r, Y_r, Z_r);
                un_obPts_c.append(xyz_r);
            }
        }
        else if (ui->cmb_y->currentIndex() > 0 && ui->cmb_z->currentIndex() == 0) {
            dim = 2;
            isTIF = true;
            outputPath = outputPath + ".tif";
            double minX = DBL_MAX;
            double maxY = -DBL_MAX;

            double maxX = -DBL_MAX;
            double minY = DBL_MAX;

            while (obIter_co.nextFeature(obFeat_co)) {
                bool valOk = false;
                double X_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toDouble(&valOk);
                double Y_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toDouble(&valOk);
                double Z_co = 0;
                if (!valOk) {
                    QString XStr_co = obFeat_co.attribute(ui->cmb_x_co->currentIndex()).toString();
                    QString YStr_co = obFeat_co.attribute(ui->cmb_y_co->currentIndex()).toString();

                    X_co = XStr_co.toDouble(&valOk);
                    Y_co = YStr_co.toDouble(&valOk);

                    if (!valOk) {
                        QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                        return;
                    }
                }

                ObPtXYZ xyz_co(X_co, Y_co, Z_co);
                co_obPts_c.append(xyz_co);
            }

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

                if (X < minX)minX = X;
                if (X > maxX)maxX = X;
                if (Y > maxY)maxY = Y;
                if (Y < minY)minY = Y;

                ObPtXYZ xyz(X, Y, Z);
                obPts_c.append(xyz);
                obPt_val.append(val);
            }

            double length = maxX - minX;
            double height = maxY - minY;

            int n_x = length / size + 2;
            int n_y = height / size + 2;

            std::cout << n_x;
            std::cout << n_y;


            if (n_x < 2 || n_x>10000 || n_y < 2 || n_y>10000) {
                QMessageBox::critical(nullptr, "Error about input grid size", "Input grid size is too small or too big!");
                return;
            }

            for (int i_y = 0; i_y < n_y; i_y++) {
                for (int i_x = 0; i_x < n_x; i_x++) {
                    double X_r = minX + size * i_x;
                    double Y_r = maxY - size * i_y;
                    double Z_r = 0;

                    ObPtXYZ xyz_r(X_r, Y_r, Z_r);
                    un_obPts_c.append(xyz_r);
                }
            }
        }
        else {
            QMessageBox::critical(nullptr, "Error about something", "Call Tel 17331242160");
            return;
        }
    }


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
        emit begin("Co-YangCZ");
        emit sendPyParams(obPt_val, co_obPt_val, obPts_c, co_obPts_c, un_obPts_c, c_list, k_num, dim, num_co, outputPath, is_nug, isTIF);
        emit getOutPath(outputPath, isTIF);
    }
}


