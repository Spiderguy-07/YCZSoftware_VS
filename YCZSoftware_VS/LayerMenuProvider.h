#ifndef QGIS_DEVLAYERTREEVIEWMENUPROVIDER_H
#define QGIS_DEVLAYERTREEVIEWMENUPROVIDER_H

#include <QObject>

#include <qgsmapcanvas.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgsmaplayer.h>
#include <qgsmapoverviewcanvas.h>

#include "./src/service/layerpropform.h"

class QAction;

struct LegendLayerAction
{
    LegendLayerAction(QAction* a, QString m, QString i, bool all)
        : action(a), menu(m), id(i), allLayers(all) {}
    QAction* action;
    QString menu;
    QString id;
    bool allLayers;
    QList<QgsMapLayer*> layers;
};
class LayerMenuProvider:public QWidget,public QgsLayerTreeViewMenuProvider
{
    Q_OBJECT
public:
    LayerMenuProvider(QgsLayerTreeView* view, QgsMapCanvas* canvas);
    ~LayerMenuProvider();

    //rewite menuProvider
    QMenu* createContextMenu() override;

protected:

    QgsLayerTreeView* mView;
    QgsMapCanvas* mCanvas;

private slots:
    void onActionLayerProp();
};



#endif // QGIS_DEVLAYERTREEVIEWMENUPROVIDER_H
