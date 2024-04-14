#include "oyangczservicedialog.h"
#include "ui_oyangczservicedialog.h"

OYangCZServiceDialog::OYangCZServiceDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OYangCZServiceDialog)
{
    ui->setupUi(this);
    
    ui->cmb_choose->setCurrentIndex(-1);
    ui->label_unknow->setVisible(false);
    ui->cmb_unknow->setVisible(false);
    ui->label_unknow_c->setVisible(false);
    ui->label_grid->setVisible(false);
    ui->line_size->setVisible(false);
    ui->xyz_widget->setVisible(false);
    //ui->label_area->setVisible(false);
    //ui->cmbRangePath->setVisible(false);

    this->initUI(project->layers<QgsVectorLayer*>());
    connect(ui->cmb_choose, &QComboBox::currentTextChanged, this, &OYangCZServiceDialog::InputChanged);
    connect(ui->btnComfirm, &QPushButton::clicked, this, &OYangCZServiceDialog::onBtnConfirmClicked);
}

OYangCZServiceDialog::~OYangCZServiceDialog()
{
    delete ui;
}

void OYangCZServiceDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    dim = 0;
    isTIF = false;
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
    }
    ui->cmbObPath->setCurrentIndex(-1);
    ui->cmb_unknow->setCurrentIndex(-1);
    connect(ui->cmbObPath, &QComboBox::currentTextChanged, this, &OYangCZServiceDialog::onCmbObPathChange);
    connect(ui->cmb_unknow, &QComboBox::currentTextChanged, this, &OYangCZServiceDialog::onCmbUnknowChange);
    connect(ui->btnBrowseOutput, &QPushButton::clicked, this, &OYangCZServiceDialog::onBtnBrowseOutputClicked);
}


void OYangCZServiceDialog::InputChanged()
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

void OYangCZServiceDialog::onCmbObPathChange()
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
    ui->cmb_y_u->setCurrentIndex(-1);
    ui->cmb_z_u->setCurrentIndex(-1);

    connect(ui->cmb_z, &QComboBox::currentTextChanged, this, &OYangCZServiceDialog::onXYZChanged);
    connect(ui->cmb_y, &QComboBox::currentTextChanged, this, &OYangCZServiceDialog::onXYZChanged);
}

void OYangCZServiceDialog::onCmbUnknowChange()
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

void OYangCZServiceDialog::onBtnBrowseOutputClicked()
{
    int choose = ui->cmb_choose->currentIndex();
    if (choose == 1)
    {
        if (ui->cmb_y->currentIndex() > 0)
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("TIF file(*.tif)"));
            ui->lineOutputPath->setText(fileName);
        }
        else if(ui->cmbObPath->currentIndex() != -1)
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("TIF file(*.csv)"));
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
        QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("TIF file(*.csv)"));
        ui->lineOutputPath->setText(fileName);
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("output file"), "untitled", tr("Unknow file(*.)"));
        ui->lineOutputPath->setText(fileName);
    }
}

void OYangCZServiceDialog::onXYZChanged()
{
    if (ui->cmb_y->currentIndex() > 0)
    {
        ui->cmb_z->setEnabled(true);
        
        ui->cmb_y_u->setEnabled(true);
        if (ui->cmb_z->currentIndex() > 0)
        {
            ui->cmb_z_u->setEnabled(true);
            ui->cmb_choose->setCurrentIndex(0);
            ui->cmb_choose->setEnabled(false);
        }
        else
        {
            ui->cmb_z_u->setCurrentIndex(-1);
            ui->cmb_z_u->setEnabled(false);
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
    }
}

void OYangCZServiceDialog::onBtnConfirmClicked()
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
        if (ui->cmb_unknow->currentIndex() == -1) {
            QMessageBox::critical(nullptr, "Error about output input", "Unknow Data Layer is not choose!");
            return;
        }
    }
    else if(ui->cmb_choose->currentIndex() == 1)
    {
        if (ui->line_size->text().isEmpty()) {
            QMessageBox::critical(nullptr, "Error about output input", "The interval size is empty!");
            return;
        }
    }

    QString outputPath = ui->lineOutputPath->text();
    int k_num = ui->sb_k_num->value();
    double c_val = ui->dsb_c_num->value();
    bool error_1 = false;
    if (ui->cb_error->isChecked())
        error_1 = true;

    QList<ObPtXYZ> obPts_c;
    QList<ObPtXYZ> un_obPts_c;
    QList<double> obPt_val;

    int obInd = ui->cmbObPath->currentIndex();
    QgsVectorLayer* obLyr = lyrs.at(obInd);
    QgsFeatureIterator obIter = obLyr->getFeatures();
    QgsFeature obFeat;

    int obInd_u = ui->cmb_unknow->currentIndex();
    QgsVectorLayer* obLyr_u = lyrs.at(obInd_u);
    QgsFeatureIterator obIter_u = obLyr_u->getFeatures();
    QgsFeature obFeat_u;

    //int obInd_r = ui->cmbRangePath->currentIndex();
    //QgsVectorLayer* obLyr_r = lyrs.at(obInd_r);
    //QgsRectangle layerExtent = obLyr->extent();

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

    this->close();
    emit begin("Ordinary YangCZ");
    emit sendPyParams(obPt_val, obPts_c, un_obPts_c, c_val, k_num, dim, outputPath, error_1, isTIF);
    emit getOutPath(outputPath, isTIF);
}
