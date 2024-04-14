#include "coanalysisdialog.h"
#include "ui_coanalysisdialog.h"

CoAnalysisDialog::CoAnalysisDialog(QgsProject* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoAnalysisDialog)
{
    ui->setupUi(this);
    initUI(project->layers<QgsVectorLayer*>());
}

CoAnalysisDialog::~CoAnalysisDialog()
{
    delete ui;
}

void CoAnalysisDialog::initUI(QVector<QgsVectorLayer*> pjLyr)
{
    for (const auto& vecLyr : pjLyr) {
        lyrs.append(vecLyr);
        ui->cmb_layer->addItem(vecLyr->name());
    }
    ui->cmb_layer->setCurrentIndex(-1);
    ui->cmb_numbers->setCurrentIndex(0);
    ui->cmb_1->setVisible(true);
    ui->cmb_2->setVisible(true);
    ui->cmb_3->setVisible(false);
    ui->cmb_4->setVisible(false);
    ui->cmb_5->setVisible(false);
    ui->cmb_6->setVisible(false);

    ui->label_1->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    connect(ui->cmb_layer, &QComboBox::currentTextChanged, this, &CoAnalysisDialog::onCmbLayerChange);
    connect(ui->cmb_numbers, &QComboBox::currentTextChanged, this, &CoAnalysisDialog::onCmbNumChange);
    connect(ui->pb_ok, &QPushButton::clicked, this, &CoAnalysisDialog::onBtnOkClicked);
}

double CoAnalysisDialog::mean(const QVector<double>& vec)
{
    double sum = 0.0;
    for (double val : vec) {
        sum += val;
    }
    return sum / vec.size();
}

double CoAnalysisDialog::pearsonCorrelation(const QVector<double>& x, const QVector<double>& y)
{
    int n = x.size();
    if (n != y.size() || n <= 1) {
        return std::numeric_limits<double>::quiet_NaN(); // Return NaN for invalid input
    }

    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0, sumY2 = 0.0;

    for (int i = 0; i < n; ++i) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
        sumY2 += y[i] * y[i];
    }

    double meanX = sumX / n;
    double meanY = sumY / n;
    double covXY = sumXY / n - meanX * meanY;
    double varX = sumX2 / n - meanX * meanX;
    double varY = sumY2 / n - meanY * meanY;

    if (varX <= 0.0 || varY <= 0.0) {
        return 0.0; // Return 0 if variance is zero
    }

    return covXY / std::sqrt(varX * varY);
}

QVector<QVector<double>> CoAnalysisDialog::computeCorrelationMatrix(const QVector<QVector<double>>& data)
{
    int n = data.size();
    QVector<QVector<double>> correlationMatrix(n, QVector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            correlationMatrix[i][j] = pearsonCorrelation(data[i], data[j]);
        }
    }

    return correlationMatrix;
}

void CoAnalysisDialog::onCmbNumChange()
{
    int fields_n = ui->cmb_numbers->currentIndex();
    switch (fields_n) {
    case 0:
    {
        ui->cmb_1->setVisible(true);
        ui->cmb_2->setVisible(true);
        ui->cmb_3->setVisible(false);
        ui->cmb_4->setVisible(false);
        ui->cmb_5->setVisible(false);
        ui->cmb_6->setVisible(false);

        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(false);
        ui->label_4->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->cmb_3->setCurrentIndex(-1);
        ui->cmb_4->setCurrentIndex(-1);
        ui->cmb_5->setCurrentIndex(-1);
        ui->cmb_6->setCurrentIndex(-1);
        break;

    }
    case 1:
    {
        ui->cmb_1->setVisible(true);
        ui->cmb_2->setVisible(true);
        ui->cmb_3->setVisible(true);
        ui->cmb_4->setVisible(false);
        ui->cmb_5->setVisible(false);
        ui->cmb_6->setVisible(false);

        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(false);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->cmb_4->setCurrentIndex(-1);
        ui->cmb_5->setCurrentIndex(-1);
        ui->cmb_6->setCurrentIndex(-1);
        break;
    }
    case 2:
    {
        ui->cmb_1->setVisible(true);
        ui->cmb_2->setVisible(true);
        ui->cmb_3->setVisible(true);
        ui->cmb_4->setVisible(true);
        ui->cmb_5->setVisible(false);
        ui->cmb_6->setVisible(false);

        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(false);
        ui->label_6->setVisible(false);
        ui->cmb_5->setCurrentIndex(-1);
        ui->cmb_6->setCurrentIndex(-1);
        break;
    }
    case 3:
    {
        ui->cmb_1->setVisible(true);
        ui->cmb_2->setVisible(true);
        ui->cmb_3->setVisible(true);
        ui->cmb_4->setVisible(true);
        ui->cmb_5->setVisible(true);
        ui->cmb_6->setVisible(false);

        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_6->setVisible(false);
        ui->cmb_6->setCurrentIndex(-1);
        break;
    }
    case 4:
    {
        ui->cmb_1->setVisible(true);
        ui->cmb_2->setVisible(true);
        ui->cmb_3->setVisible(true);
        ui->cmb_4->setVisible(true);
        ui->cmb_5->setVisible(true);
        ui->cmb_6->setVisible(true);

        ui->label_1->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_5->setVisible(true);
        ui->label_6->setVisible(true);
        break;
    }
    }
        

}

void CoAnalysisDialog::onBtnOkClicked()
{
    int fields_n = ui->cmb_numbers->currentIndex() + 2;
    QStringList labels;
    // Check if there are enough fields selected
    if (fields_n > 0 && fields_n <= 6) {
        QVector<QVector<double>> dataMatrix(fields_n);

        for (int i = 0; i < fields_n; ++i) {
            QComboBox* comboBox = nullptr;
            switch (i) {
            case 0: comboBox = ui->cmb_1; break;
            case 1: comboBox = ui->cmb_2; break;
            case 2: comboBox = ui->cmb_3; break;
            case 3: comboBox = ui->cmb_4; break;
            case 4: comboBox = ui->cmb_5; break;
            case 5: comboBox = ui->cmb_6; break;
            }

            if (comboBox) {
                int lyrIndex = ui->cmb_layer->currentIndex();
                QgsVectorLayer* lyr = lyrs.at(lyrIndex);
                int fieldIndex = comboBox->currentIndex();
                QgsFeatureIterator obIter = lyr->getFeatures();
                QgsFeature obFeat;
                labels.append(comboBox->currentText());

                QVector<double> attrValues;

                while (obIter.nextFeature(obFeat)) {
                    bool valOk = false;
                    double val = obFeat.attribute(fieldIndex).toDouble(&valOk);
                    if (!valOk) {
                        QString valStr = obFeat.attribute(fieldIndex).toString();
                        val = valStr.toDouble(&valOk);
                        if (!valOk) {
                            QMessageBox::critical(this, "Illegal data type", "Data type of val should be number.");
                            return;
                        }
                    }
                    attrValues.append(val);
                }

                dataMatrix[i] = attrValues;
            }
        }

        QVector<QVector<double>> correlationMatrix = computeCorrelationMatrix(dataMatrix);
        std::cout << "end";

        CorrelationMatrixWidget* cowidget = new CorrelationMatrixWidget();
        cowidget->setCorrelationMatrix(correlationMatrix);
        cowidget->setLabels(labels);
        cowidget->setWindowTitle("Correlation Coefficient");
        QIcon windowIcon = this->windowIcon();
        cowidget->setWindowIcon(windowIcon);
        cowidget->show();
        // Now you have the correlation matrix, you can use it as needed
    }
    else {
        // Handle error: not enough fields selected
    }
}

void CoAnalysisDialog::onCmbLayerChange()
{
    ui->cmb_1->clear();
    ui->cmb_2->clear();
    ui->cmb_3->clear();
    ui->cmb_4->clear();
    ui->cmb_5->clear();
    ui->cmb_6->clear();

    int index = ui->cmb_layer->currentIndex();
    QgsVectorLayer* lyr = lyrs.at(index);
    const QgsAttributeList attrList = lyr->attributeList();
    QgsAttributeList::const_iterator it = attrList.constBegin();
    for (; it != attrList.constEnd(); it++) {
        //QMessageBox::warning(this, "111", lyr->attributeDisplayName(*it));
        ui->cmb_1->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_2->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_3->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_4->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_5->addItem(lyr->attributeDisplayName(*it));
        ui->cmb_6->addItem(lyr->attributeDisplayName(*it));
    }

    ui->cmb_1->setCurrentIndex(0);
    ui->cmb_2->setCurrentIndex(0);
    ui->cmb_3->setCurrentIndex(-1);
    ui->cmb_4->setCurrentIndex(-1);
    ui->cmb_5->setCurrentIndex(-1);
    ui->cmb_6->setCurrentIndex(-1);

}