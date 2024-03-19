#include "layerpropform.h"
#include "ui_layerpropform.h"

LayerPropForm::LayerPropForm(QgsMapLayer* _mlayer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerPropForm)
{
    this->layer = _mlayer;
    ui->setupUi(this);
    initUI();
}

LayerPropForm::~LayerPropForm()
{
    delete ui;
}

void LayerPropForm::initUI()
{
    QTabBar* layerbar = ui->tabWidget->findChild<QTabBar*>();
    layerbar->hide();

    QTabBar* renderBar = ui->comboTabWidget->findChild<QTabBar*>();
    renderBar->hide();

    ui->listWidget->setCurrentRow(0);
    this->initInformationTab();
    decideRasterNVector(0);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &LayerPropForm::listWidgetItemClicked);
    connect(ui->okPb, &QPushButton::clicked, [this] { renderApplyPbClicked(true); });
    connect(ui->cancelPb, &QPushButton::clicked, this, &LayerPropForm::close);
    connect(ui->applyPb, &QPushButton::clicked, [this] { renderApplyPbClicked(false); });
    connect(ui->vecterRenderCB, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LayerPropForm::vecterRenderCBChanged);
}


void LayerPropForm::initInformationTab()
{
    /*if (QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer*>(layer))
    {
        QMap<QString, QString> rasterLayerDict = getRasterLayerAttrs(rasterLayer);
        rasterNameLabel->setText(rasterLayerDict["name"]);
        rasterSourceLabel->setText(rasterLayerDict["source"]);
        rasterMemoryLabel->setText(rasterLayerDict["memory"]);
        rasterExtentLabel->setText(rasterLayerDict["extent"]);
        rasterWidthLabel->setText(rasterLayerDict["width"]);
        rasterHeightLabel->setText(rasterLayerDict["height"]);
        rasterDataTypeLabel->setText(rasterLayerDict["dataType"]);
        rasterBandNumLabel->setText(rasterLayerDict["bands"]);
        rasterCrsLabel->setText(rasterLayerDict["crs"]);
        rasterRenderWidget = new QgsRendererRasterPropertiesWidget(rasterLayer, nullptr, this);
        layerRenderLayout->addWidget(rasterRenderWidget);
    }*/
    if (QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>(layer))
    {
        QMap<QString, QString> vectorLayerDict = getVectorLayerAttrs(vectorLayer);
        ui->vectorNameLabel->setText(vectorLayerDict["name"]);
        ui->vectorSourceLabel->setText(vectorLayerDict["source"]);
        ui->vectorMemoryLabel->setText(vectorLayerDict["memory"]);
        ui->vectorExtentLabel->setText(vectorLayerDict["extent"]);
        ui->vectorGeoTypeLabel->setText(vectorLayerDict["geoType"]);
        ui->vectorFeatureNumLabel->setText(vectorLayerDict["featureNum"]);
        ui->vectorEncodingLabel->setText(vectorLayerDict["encoding"]);
        ui->vectorCrsLabel->setText(vectorLayerDict["crs"]);
        ui->vectorDpLabel->setText(vectorLayerDict["dpSource"]);

        vectorSingleRenderWidget = new QgsSingleSymbolRendererWidget(vectorLayer, QgsStyle::defaultStyle(), vectorLayer->renderer());
        ui->singleRenderLayout->addWidget(vectorSingleRenderWidget);

        vectorCateGoryRenderWidget = new QgsCategorizedSymbolRendererWidget(vectorLayer, QgsStyle::defaultStyle(), vectorLayer->renderer());
        ui->cateRenderLayout->addWidget(vectorCateGoryRenderWidget);
    }
}

QMap<QString, QString> LayerPropForm::getVectorLayerAttrs(QgsVectorLayer* vectorLayer)
{
    QgsVectorDataProvider* vdp = vectorLayer->dataProvider();
    QgsCoordinateReferenceSystem crs = vectorLayer->crs();
    QgsRectangle extent = vectorLayer->extent();

    QMap<QString, QString> resDict{
        {"name", vectorLayer->name()},
        {"source", vectorLayer->source()},
        {"memory", QString::number(QFileInfo(vectorLayer->source()).size())},
        {"extent", QString("min:[%1,%2]; max:[%3,%4]").arg(extent.xMinimum(), 6, 'f').arg(extent.yMinimum(), 6, 'f').arg(extent.xMaximum(), 6, 'f').arg(extent.yMaximum(), 6, 'f')},
        {"geoType", QgsWkbTypes::geometryDisplayString(vectorLayer->geometryType())},
        {"featureNum", QString::number(vectorLayer->featureCount())},
        {"encoding", vdp->encoding()},
        {"crs", crs.description()},
        {"dpSource", vdp->description()} };

    return resDict;
}

void LayerPropForm::decideRasterNVector(int index)
{
    if (index == 0)
    {
        if (qobject_cast<QgsRasterLayer*>(layer))
            ui->tabWidget->setCurrentIndex(0);
        else if (qobject_cast<QgsVectorLayer*>(layer))
            ui->tabWidget->setCurrentIndex(1);
    }
    else if (index == 1)
    {
        if (qobject_cast<QgsRasterLayer*>(layer))
            ui->tabWidget->setCurrentIndex(2);
        else if (qobject_cast<QgsVectorLayer*>(layer))
            ui->tabWidget->setCurrentIndex(3);
    }
}

void LayerPropForm::listWidgetItemClicked(QListWidgetItem* item)
{
    int tempIndex = ui->listWidget->row(item);
    decideRasterNVector(tempIndex);
}
void LayerPropForm::vecterRenderCBChanged(int index)
{
    ui->comboTabWidget->setCurrentIndex(index);
}

void LayerPropForm::renderApplyPbClicked(bool needClose)
{
    if (ui->tabWidget->currentIndex() <= 1)
    {
        qDebug() << "没有在视图里，啥也不干";
    }
    else if (QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer*>(layer))
    {
        rasterRenderWidget->apply();
    }
    else if (QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>(layer))
    {
        qDebug() << "矢量渲染";
        if (ui->comboTabWidget->currentIndex() == 0)
        {
            QgsFeatureRenderer* renderer = vectorSingleRenderWidget->renderer();
            vectorLayer->setRenderer(renderer);
        }
        else if (ui->comboTabWidget->currentIndex() == 1)
        {
            QgsFeatureRenderer* renderer = vectorCateGoryRenderWidget->renderer();
            vectorLayer->setRenderer(renderer);
        }
        else
        {

        }

        vectorLayer->triggerRepaint();
    }
    emit mapCanvasRefreshed();
    if (needClose)
    {
        close();
    }
}

