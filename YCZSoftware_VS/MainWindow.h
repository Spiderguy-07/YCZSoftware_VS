#pragma once

#include <QtWidgets/QMainWindow>
#include <qprocess.h>
#include <qgsmapcanvas.h>
#include <qgslayertree.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgslayertreenode.h>
#include <qgsrasterlayer.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>
#include <qgsidentifymenu.h>
#include <qsplitter.h>  
#include <qgsmapoverviewcanvas.h>
#include <qgsattributetableview.h>	
#include <qgsvectorlayercache.h>
#include <qgsattributetablemodel.h>
#include <qgsattributetablefiltermodel.h>
#include <qgsmaptoolemitpoint.h>
#include <qgsmaptoolidentifyfeature.h>	
#include <qgshighlight.h>	
#include <qgsvectorlayer.h>
#include <qmessagebox.h>
#include <qgspointxy.h>	
#include <qgsfeature.h>	
#include <qgis.h>	
#include <qgsfield.h>	

#include "./src/service/MainWindow3D.h"
#include "./src/service/Lyr3DSelectDialog.h"
#include "./src/service/YCZFilterServiceDialog.h"
#include "./src/service/YCZFilterPyThread.h"
#include "./src/service/aboutsoftdialog.h"
#include "./src/service/sqldialog.h"

#include "ui_MainWindow.h"
#include <qgsscalecombobox.h>
#include <qprogressbar.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Json::Value config, QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::YCZSoftware_VSClass ui;
    Json::Value _mConfig;
    Json::Value _mAppConfig;
    QgsProject* _mProject;
    QgsDockWidget* _mLayerDock;
    QgsDockWidget* _mEagleEyeDock;
    QgsMapCanvas* _mCanvas2D;
    QgsMapOverviewCanvas* _mEagleEyeMapOverview;
    QTabWidget* _mCenterTabWidget;
    QgsLayerTreeView* _mLayerTreeView;
    QgsLayerTreeMapCanvasBridge* _mLayerTreeCanvasBridge;
    QgsAttributeTableFilterModel* _mAttrTableFilterModel;
    QgsVectorLayerCache* _mVectorLayerCache;
    QgsAttributeTableModel* _mAttrTableModel;
    QgsHighlight* _mHighlight;

    QgsMapToolZoom* _mZooomInTool, * _mZoomOutTool;
    QgsMapToolPan* _mPanTool;
    QgsMapToolEmitPoint* _mToolEmitPoint;
    QgsMapToolIdentifyFeature* _mToolIdentifyFeature;

    QLabel* m_scaleLabel; // 在状态栏中显示“scale 1:”
    QgsScaleComboBox* m_scaleEdit; //! 在状态栏中显示比例尺值
    QProgressBar* m_progressBar;
    QLabel* m_coordsLabel; //! 在状态栏显示"Coordinate / Extent"
    QLineEdit* m_coordsEdit; //! 在状态栏显示地理坐标

    bool isProjectOpened;

    void initUi();

    void createStatusBar();

private slots:
    bool onActionOpen();
    bool onActionNew();
    bool onActionSave();
    bool onActionSaveAs();
    bool onActionImportShpTriggered();
    bool onActionOpen3DWindowTriggered();
    bool onActionImportXYZTriggered();
    bool onActionYCZFilterTriggered();
    bool onActionAbout();

    void onSendSelected3DPathEmitted(QString path);
    void onSendSelected3DLyrEmitted(QString lyrName);

    void onYCZFilterParamsSended(QList<ObPt> obPts, QList<UnobPt> unobPts, QString outputPath);

    void onActionPanTriggered();
    void onActionZoomInTriggered();
    void onActionZoomOutTriggered();
    void onActionZoomFullTriggered();
    void onActionUnsetTriggered();
    void onActionShowTableTriggered();
    void onActionFeatureInfo();
    void onActionClearHighlight();
    void onActionEmitPoint();
    void onActionSelectPoint();
    void onActionSelectVector();
    void onActionSerchSQL();

    void onCanvasRefresh();

    void onClickSQLSearch();

};
