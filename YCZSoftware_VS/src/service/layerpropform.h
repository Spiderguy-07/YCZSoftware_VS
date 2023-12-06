#ifndef LAYERPROPFORM_H
#define LAYERPROPFORM_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabBar>
#include <QListWidgetItem>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>

#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsstyle.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgswkbtypes.h>
#include <qgsfeature.h>
#include <qgsrasterrenderer.h>
#include <qgsrendererrasterpropertieswidget.h>
#include <qgssinglesymbolrendererwidget.h>
#include <qgscategorizedsymbolrendererwidget.h>

#include "ui_layerpropform.h"
//#include "LayerUtils.h"
#include <QWidget>

namespace Ui {
class LayerPropForm;
}

class LayerPropForm : public QWidget
{
    Q_OBJECT

public:
    LayerPropForm(QgsMapLayer* _mlayer, QWidget *parent = nullptr);
    ~LayerPropForm();

    void initUI();

    void initInformationTab();
    QMap<QString, QString> getVectorLayerAttrs(QgsVectorLayer* vectorLayer);
    void decideRasterNVector(int index);

private:
    Ui::LayerPropForm *ui;

    QgsMapLayer* layer;
    QgsRendererRasterPropertiesWidget* rasterRenderWidget;
    QgsSingleSymbolRendererWidget* vectorSingleRenderWidget;
    QgsCategorizedSymbolRendererWidget* vectorCateGoryRenderWidget;

private slots:
    void listWidgetItemClicked(QListWidgetItem* item);
    void vecterRenderCBChanged(int index);
    void renderApplyPbClicked(bool needClose);

signals:
    void mapCanvasRefreshed();
};

#endif // LAYERPROPFORM_H
