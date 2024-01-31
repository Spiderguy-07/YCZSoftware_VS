#pragma once

#include <QtWidgets/QMainWindow>
#include <QAbstractItemModel>
#include <QTextEdit>
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
#include "./src/service/oyczFilterServiceDialog.h"
#include "./src/service/YCZFilterPyThread2D.h"
#include "./src/service/aboutsoftdialog.h"
#include "./src/service/sqldialog.h"
#include "Project.h"

#include "ui_MainWindow.h"
#include <qgsscalecombobox.h>
#include <qprogressbar.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Json::Value config, QWidget* parent = nullptr);
    ~MainWindow();
    static  MainWindow* instance();

private:
    Ui::YCZSoftware_VSClass ui;
    Json::Value _mConfig;
    Json::Value _mAppConfig;
    static MainWindow* sInstance;
    //QgsProject* _mProject;
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

    QLabel* m_workLabel;

    QLabel* m_scaleLabel; // 在状态栏中显示“scale 1:”
    QgsScaleComboBox* m_scaleEdit; //! 在状态栏中显示比例尺值
    QProgressBar* m_progressBar;
    QLabel* m_coordsLabel; //! 在状态栏显示"Coordinate / Extent"
    QLineEdit* m_coordsEdit; //! 在状态栏显示地理坐标

    QString imgPath;

    //QTextEdit* m_textEditLog;
    //QDockWidget* m_dockWidgetLog;

    bool isProjectOpened;

    void initUi();
    void PythonInit();
    void connectFunc();

    void addVectorLayer(const QString& filePath);
    void addRasterLayer(const QString& filePath);
    void dragEnterEvent(QDragEnterEvent* fileData) override;
    void dropEvent(QDropEvent* fileData) override;

    void createStatusBar();

    bool windowModified();
    bool saveProject();
    void closeProject();
    void setWindowTitle();
    void openProject(const QString& fileName);

private slots:
    bool onActionOpen();
    bool onActionNew();
    bool onActionSave();
    bool onActionSaveAs();
    bool onActionExit();
    bool onActionImportShpTriggered();
    bool onActionOpen3DWindowTriggered();
    bool onActionImportXYZTriggered();
    bool onActionYCZFilterTriggered();
    bool onActionOYCZFilterTriggered();
    bool onActionAbout();

    void onSendSelected3DPathEmitted(QString path);
    void onSendSelected3DLyrEmitted(QString lyrName);

    void onYCZFilterParamsSended(QList<ObPt> obPts, QList<UnobPt> unobPts, QString outputPath);
    void onOYCZFilterParamsSended(QList<ObPt2D> obpts, QList<Range2D> rangeA, QString outputPath, double s, int k, double c, bool e);

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
    void onMapRefresh();

    void onClickSQLSearch();

    void onGetPath(QString path);
    void onImportImg(PyObject* result_re);

};
