#include "MainWindow.h"
#include "./src/macro.h"
#include "./src/service/LoadingXYZDataDialog.h"
#include "LayerMenuProvider.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow* MainWindow::sInstance = nullptr;

MainWindow::MainWindow(Json::Value config, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->_mAppConfig = config;
    this->_mConfig = config[MAINWINDOW_2D];
    this->_mHighlight = nullptr;
    this->isProjectOpened = false;
    this->_mprogress = nullptr;
    Py_SetPythonHome(L"E:/Anaconda3/InstallPos/envs/cluster");
    //this->setupEagleEyeMap();
    this->initUi();
    this->PythonInit();
}

MainWindow::~MainWindow()
{}

MainWindow* MainWindow::instance()
{
    if (!sInstance)
    {
        sInstance = new MainWindow(nullptr);
    }
    return sInstance;
}

void MainWindow::initUi()
{
    QString iconDir = "./icon/";

    this->setWindowIcon(QIcon(iconDir + "YangCZ.png"));

    ui.actionImportShp->setIcon(QIcon(iconDir + "OpenHyperlink_32x32.png"));
    ui.actionPan->setIcon(QIcon(iconDir + "PanTool32.png"));
    ui.actionZoomOut->setIcon(QIcon(iconDir + "ZoomOut_32x32.png"));
    ui.actionZoomIn->setIcon(QIcon(iconDir + "ZoomIn_32x32.png"));
    ui.actionZoomFull->setIcon(QIcon(iconDir + "FullExtent_32x32.png"));
    ui.actionShowTable->setIcon(QIcon(iconDir + "Table_32x32.png"));
    ui.actionUnset->setIcon(QIcon(iconDir + "Pointer_32x32.png"));
    ui.actionFeatureInfo->setIcon(QIcon(iconDir + "Info_32x32.png"));
    ui.actionClearSelect->setIcon(QIcon(iconDir + "No_Border_32x32.png"));
    ui.actionEmitPoint->setIcon(QIcon(iconDir + "GeoPoint_32x32.png"));
    //this->m_texteditlog = new qtextedit(this);
    //this->m_dockwidgetlog = new qdockwidget(this);
    //this->m_texteditlog->setreadonly(true);
    //this->m_dockwidgetlog->setwidget(m_texteditlog);
    //adddockwidget(qt::dockwidgetarea::bottomdockwidgetarea, m_dockwidgetlog);

    // set window size
    this->setGeometry(this->_mConfig[X_LOCATION].asInt(),
        this->_mConfig[Y_LOCATION].asInt(),
        this->_mConfig[WIDTH].asInt(),
        this->_mConfig[HEIGHT].asInt());

    //this->setWindowTitle(this->_mConfig[TITLE].asCString());  // set title  1.20修改

    //setWindowFlags(Qt::FramelessWindowHint);//隐藏边框 


    this->setWindowTitle();

    // create left dock
    this->_mLayerDock = new QgsDockWidget(this);
    this->_mLayerDock->setWindowTitle(this->_mConfig[LAYER_DOCK_TITLE].asCString());
    this->_mLayerDock->setObjectName(this->_mConfig[LAYER_DOCK_OBJ_NAME].asCString());
    this->addDockWidget(Qt::LeftDockWidgetArea, this->_mLayerDock);

    // Set minimum and maximum width for the dock
    this->_mLayerDock->setMinimumWidth(500);  // Adjust with your desired minimum width
    this->_mLayerDock->setMaximumWidth(1000);  // Adjust with your desired maximum width

    // Set minimum and maximum hight for the dock
    this->_mLayerDock->setMinimumHeight(300);  // Adjust with your desired minimum height
    this->_mLayerDock->setMaximumHeight(550);  // Adjust with your desired maximum height

    // create EagleEyeView dock
    this->_mEagleEyeDock = new QgsDockWidget(this);
    this->_mEagleEyeDock->setWindowTitle("EagleEye");
    this->_mEagleEyeDock->setObjectName(this->_mConfig[EagleEye_DOCK_OBJ_NAME].asCString());
    this->addDockWidget(Qt::LeftDockWidgetArea, this->_mEagleEyeDock);


    // create container of canvas
    this->_mCenterTabWidget = new QTabWidget(this);

    // create 2D canvas
    this->_mCanvas2D = new QgsMapCanvas{};
    this->_mCanvas2D->setCanvasColor(QColor(this->_mConfig[CANVAS_2D_BGC_R].asInt(),
        this->_mConfig[CANVAS_2D_BGC_G].asInt(),
        this->_mConfig[CANVAS_2D_BGC_B].asInt()));
    this->_mCanvas2D->setSelectionColor(QColor(this->_mConfig[CANVAS_2D_STC_R].asInt(),
        this->_mConfig[CANVAS_2D_STC_G].asInt(),
        this->_mConfig[CANVAS_2D_STC_B].asInt()));
    // add canvas to container
    this->_mCenterTabWidget->addTab(this->_mCanvas2D, this->_mConfig[MAP_VIEW_TITLE].asCString());

    this->setCentralWidget(this->_mCenterTabWidget);

    // create layer tree
    this->_mLayerTreeView = new QgsLayerTreeView(this);
    this->_mLayerTreeView->setObjectName(this->_mConfig[LAYER_TREE_OBJ_NAME].asCString());

    //create eagle eye canvas
    this->_mEagleEyeMapOverview = new QgsMapOverviewCanvas(_mEagleEyeDock,_mCanvas2D);

    this->createStatusBar();

    // TODO: bind layer tree model to project, after functions of project is complete
    QgsLayerTreeModel* model = new QgsLayerTreeModel(Project::instance()->layerTree(), this);
    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
    model->setFlag(QgsLayerTreeModel::AllowNodeRename);
    model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
    model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
    model->setAutoCollapseLegendNodes(this->_mConfig[LAYER_TREE_COLLAPSE_NUM].asInt());

    this->_mLayerTreeView->setModel(model);
    this->_mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(Project::instance()->layerTree(),this->_mCanvas2D, this);
    this->_mLayerDock->setWidget(this->_mLayerTreeView);
    this->_mEagleEyeDock->setWidget(this->_mEagleEyeMapOverview);

    //new Tool
    _mZooomInTool = new QgsMapToolZoom(_mCanvas2D, false);
    _mZoomOutTool = new QgsMapToolZoom(_mCanvas2D, true);
    _mPanTool = new QgsMapToolPan(_mCanvas2D);

    //layermenuprovider
    _mLayerTreeView->setMenuProvider(new LayerMenuProvider(_mLayerTreeView, _mCanvas2D));

    this->resultPath = "";
    this->isTIF = false;

    setAcceptDrops(true);

    this->connectFunc();

    this->showMaximized();
    //_mprogress = new yczprogressDialog(this);
    //_mprogress->show();

    //yczprogressDialog* newdialog = new yczprogressDialog(nullptr);
    //newdialog->show();

}

void MainWindow::connectFunc()
{
    // connect signals and slots
    this->connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    this->connect(ui.actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    this->connect(ui.actionSave, &QAction::triggered, this, &MainWindow::onActionSave);
    this->connect(ui.actionSaveAs, &QAction::triggered, this, &MainWindow::onActionSaveAs);
    this->connect(ui.actionExit, &QAction::triggered, this, &MainWindow::onActionExit);
    this->connect(ui.actionImportShp, &QAction::triggered, this, &MainWindow::onActionImportShpTriggered);
    this->connect(ui.actionOpen3DWindow, &QAction::triggered, this, &MainWindow::onActionOpen3DWindowTriggered);
    this->connect(ui.actionImportXYZ, &QAction::triggered, this, &MainWindow::onActionImportXYZTriggered);
    //this->connect(ui.actionYCZFilter, &QAction::triggered, this, &MainWindow::onActionYCZFilterTriggered);
    //this->connect(ui.actionCreatRster, &QAction::triggered, this, &MainWindow::onActionOYCZFilterTriggered);
    //this->connect(ui.actionCheckAccuracy, &QAction::triggered, this, &MainWindow::onActionYCZFilterTriggered);
    this->connect(ui.actionHistogram, &QAction::triggered, this, &MainWindow::onActionHistogram);
    this->connect(ui.actionQQ_Plot, &QAction::triggered, this, &MainWindow::onActionQQPlot);
    this->connect(ui.actionBox_Plot, &QAction::triggered, this, &MainWindow::onActionBoxPlot);
    this->connect(ui.actionLog_Transform, &QAction::triggered, this, &MainWindow::onActionLog);
    this->connect(ui.actionInverse_Log_Transform, &QAction::triggered, this, &MainWindow::onActionInLog);
    this->connect(ui.actionCorrelation_Analysis, &QAction::triggered, this, &MainWindow::onActionCoAnalyse);
    
    this->connect(ui.actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);
    this->connect(ui.actionPan, &QAction::triggered, this, &MainWindow::onActionPanTriggered);
    this->connect(ui.actionZoomOut, &QAction::triggered, this, &MainWindow::onActionZoomOutTriggered);
    this->connect(ui.actionZoomFull, &QAction::triggered, this, &MainWindow::onActionZoomFullTriggered);
    this->connect(ui.actionZoomIn, &QAction::triggered, this, &MainWindow::onActionZoomInTriggered);
    this->connect(ui.actionUnset, &QAction::triggered, this, &MainWindow::onActionUnsetTriggered);
    this->connect(ui.actionShowTable, &QAction::triggered, this, &MainWindow::onActionShowTableTriggered);
    this->connect(ui.actionClearSelect, &QAction::triggered, this, &MainWindow::onActionClearHighlight);
    this->connect(ui.actionEmitPoint, &QAction::triggered, this, &MainWindow::onActionEmitPoint);
    this->connect(ui.actionFeatureInfo, &QAction::triggered, this, &MainWindow::onActionFeatureInfo);
    this->connect(ui.actionSelectPoint, &QAction::triggered, this, &MainWindow::onActionSelectPoint);
    this->connect(ui.actionSelectVector, &QAction::triggered, this, &MainWindow::onActionFeatureInfo);
    this->connect(ui.actionSerchSQL, &QAction::triggered, this, &MainWindow::onActionSerchSQL);
    this->connect(this->_mCanvas2D, SIGNAL(layersChanged()), this, SLOT(onCanvasRefresh()));

    this->connect(ui.actionTo_be_developed, &QAction::triggered, this, &MainWindow::onWaiting);

    auto lambdaSetModified = [this] { setWindowModified(true); };
    connect(_mLayerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::addedChildren, this, lambdaSetModified);
    connect(_mLayerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::addedChildren, this, &MainWindow::onMapRefresh);
    connect(_mLayerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::removedChildren, this, lambdaSetModified);

    this->connect(ui.actionOrdinary_YCZ, &QAction::triggered, this, &MainWindow::onActionOYangCZ);
    this->connect(ui.actionYCZF, &QAction::triggered, this, &MainWindow::onActionYangCZF);
    this->connect(ui.actionGYCZ, &QAction::triggered, this, &MainWindow::onActionGYangCZ);
    this->connect(ui.actionSTYCZ, &QAction::triggered, this, &MainWindow::onActionSTYangCZ);
    this->connect(ui.actionCoYCZ, &QAction::triggered, this, &MainWindow::onActionCoYangCZ);

    this->connect(ui.actionAmoeba_G, &QAction::triggered, this, &MainWindow::onActionAmoebaG);
    this->connect(ui.actionHTMSCAN, &QAction::triggered, this, &MainWindow::onActionHTMSCAN);
}

void MainWindow::addVectorLayer(const QString& filePath)
{
    QgsVectorLayer* vectorLayer = new QgsVectorLayer(filePath, QFileInfo(filePath).baseName(), "ogr");

    if (vectorLayer->isValid())
    {
        QgsProject::instance()->addMapLayer(vectorLayer);
        Project::instance()->addLayer(vectorLayer);
    }
    else
    {
        QMessageBox::about(this, "ERROR", QString("CAN NOT LOADING：%1").arg(filePath));
        delete vectorLayer;
    }
}

void MainWindow::addRasterLayer(const QString& filePath)
{
    QgsRasterLayer* rasterLayer = new QgsRasterLayer(filePath, QFileInfo(filePath).baseName(), "gdal");

    if (rasterLayer && rasterLayer->isValid())
    {
        QgsProject::instance()->addMapLayer(rasterLayer);
        // 如果你需要在Project类中添加图层，请确保你的Project类有相应的addLayer函数
        Project::instance()->addLayer(rasterLayer);
    }
    else
    {
        QMessageBox::about(this, "ERROR", QString("CAN NOT LOADING：%1").arg(filePath));
        delete rasterLayer;
    }
}


void MainWindow::dragEnterEvent(QDragEnterEvent* fileData)
{
    if (fileData->mimeData()->hasUrls())
        fileData->accept();
    else
        fileData->ignore();
}

void MainWindow::dropEvent(QDropEvent* fileData)
{
    const QMimeData* mimeData = fileData->mimeData();
    QStringList filePathList;

    for (const QUrl& url : mimeData->urls())
    {
        QString filePath = url.path().mid(1);
        filePathList.append(filePath.replace("/", "//"));
    }

    for (const QString& filePath : filePathList)
    {
        if (filePath.endsWith(".tif", Qt::CaseInsensitive) ||
            filePath.endsWith(".tiff", Qt::CaseInsensitive) ||
            filePath.endsWith(".png", Qt::CaseInsensitive) ||
            filePath.endsWith(".jpg", Qt::CaseInsensitive) ||
            filePath.endsWith(".pdf", Qt::CaseInsensitive))
        {
            addRasterLayer(filePath);
        }
        else if (filePath.endsWith(".shp", Qt::CaseInsensitive) ||
            filePath.endsWith(".gpkg", Qt::CaseInsensitive) ||
            filePath.endsWith(".geojson", Qt::CaseInsensitive) ||
            filePath.endsWith(".kml", Qt::CaseInsensitive))
        {
            addVectorLayer(filePath);
        }
        else if (filePath.isEmpty())
        {
            // Do nothing
        }
        else
        {
            QMessageBox::about(this, "Warning", QString("%1 NOT SUPPORT, support SHP file.").arg(filePath));
        }
    }
}

bool MainWindow::onActionOpen()
{
    if (!windowModified())
    {
        return false;
    }

    closeProject();

    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open Project File"), {}, QString("YangCZ Software Project File(*.ycz)"));
    if (fileName.isEmpty())
    {
        return false;
    }

    openProject(fileName);

    //QList<QgsMapLayer*> mlayers = _mCanvas2D->layers();

    //_mEagleEyeMapOverview->setLayers(mlayers);
    //_mEagleEyeMapOverview->refresh();
    onCanvasRefresh();
    return true;
}

bool MainWindow::onActionNew()
{
    //qstring projectpath = qfiledialog::getsavefilename(this, "select project location", qdir::homepath(), "qgis project files (*.qgs)");

    //if (!projectpath.isempty()) {
    //    qgsproject project;
    //    project.setfilename(projectpath);
    //    project.write();
    //}
    //this->isprojectopened = true;
    if (!windowModified())
    {
        return false;
    }
    closeProject();
    setWindowTitle();
    setWindowModified(false);
    onMapRefresh();
    onCanvasRefresh();
    return true;

}

bool MainWindow::onActionSave()
{
    if (!saveProject())
    {
        return false;
    }

    this->m_workLabel->setText(tr("Save project successfully."));
    setWindowModified(false);
    setWindowTitle();
    return true;
}

bool MainWindow::onActionSaveAs()
{
    const QString& fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), QString(), QString("YangCZ Software Project File(*.ycz)"));

    if (fileName.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo(fileName);
    Project::instance()->setFile(fileName);
    Project::instance()->setName(fileInfo.baseName());

    if (!Project::instance()->write())
    {
        return false;
    }
    this->m_workLabel->setText(tr("Save project successfully."));
    setWindowModified(false);
    setWindowTitle();
    return true;
}

bool MainWindow::onActionImportShpTriggered()
{
    QString title = QString(this->_mConfig[OPEN_VEC_DIALOG_TITLE].asCString());
    QString filterName = QString(this->_mConfig[VEC_FILTER_NAME].asCString());
    QgsProject* project = QgsProject::instance();

    QList<QgsMapLayer*> lyrsToBeAdded = FileLoadingService::loadVecLayers(title, filterName, project, this);
    if (lyrsToBeAdded.size() <= 0) {
        return false;
    }
    project->addMapLayers(lyrsToBeAdded);
    Project::instance()->addLayers(lyrsToBeAdded);
    
    return true;
    
}

bool MainWindow::onActionOpen3DWindowTriggered()
{
    QgsProject* project = QgsProject::instance();
    QVector<QgsVectorLayer*> vecLyrs = project->layers<QgsVectorLayer*>();
    QStringList lyrNameList;
    for (const auto &vecLyr : vecLyrs){
        lyrNameList.append(vecLyr->name());
    }
    
    Lyr3DSelectDialog* lyr3DDialog = new Lyr3DSelectDialog(this->_mAppConfig[LYR_3D_SELECT_DIALOG], lyrNameList, this);
    this->connect(lyr3DDialog, &Lyr3DSelectDialog::sendSelectedFilePath, this, &MainWindow::onSendSelected3DPathEmitted);
    this->connect(lyr3DDialog, &Lyr3DSelectDialog::sendSelectedLyrName, this, &MainWindow::onSendSelected3DLyrEmitted);
    lyr3DDialog->show();

    return true;
}

bool MainWindow::onActionImportXYZTriggered()
{
    QString title = QString(this->_mConfig[OPEN_CSV_DIALOG_TITLE].asCString());
    QString filterName = QString(this->_mConfig[CSV_FILTER_NAME].asCString());
    QStringList fileList = FileLoadingService::openCSVFileSelectDialog(title, filterName, this);
    if (fileList.isEmpty()) {
        return false;
    }

    QString path = fileList.at(0);
    LoadingXYZDataDialog* loadingXYZDialog = new LoadingXYZDataDialog(path, Project::instance(), this);
    loadingXYZDialog->show();
    _mCanvas2D->refresh();
    _mEagleEyeMapOverview->refresh();
    return true;
}
/*
bool MainWindow::onActionYCZFilterTriggered()
{
    QgsProject* project = QgsProject::instance();
    YCZFilterServiceDialog* yczFilterDialog = new YCZFilterServiceDialog(project, this);
    connect(yczFilterDialog, &YCZFilterServiceDialog::sendPyParams, this, &MainWindow::onYCZFilterParamsSended);
    yczFilterDialog->show();
    return false;
}

bool MainWindow::onActionOYCZFilterTriggered()
{
    QgsProject* project = QgsProject::instance();
    OYCZFilterServiceDialog* oyczFilterDialog = new OYCZFilterServiceDialog(project,this);
    connect(oyczFilterDialog, &OYCZFilterServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(oyczFilterDialog, &OYCZFilterServiceDialog::sendPyParams, this, &MainWindow::onOYCZFilterParamsSended);
    
    //QString path = oyczFilterDialog->getOutPath();
    connect(oyczFilterDialog, &OYCZFilterServiceDialog::getOutPath, this, &MainWindow::onGetPath);

    
   // connect(oyczFilterDialog, &OYCZFilterServiceDialog::begin, this, )
    oyczFilterDialog->show();

    
    return false;
}
*/
bool MainWindow::onActionAbout()
{
    AboutSoftDialog* abloutDialog = new AboutSoftDialog(this);
    return true;
}

void MainWindow::onActionClearHighlight()
{
    if (this->_mHighlight)
    {
        delete this->_mHighlight;
    }
    this->_mHighlight = nullptr;

    QList< QgsMapLayer* > 	layers = _mCanvas2D->layers();
    foreach(QgsMapLayer * l, layers)
    {
        QgsVectorLayer* l1 = qobject_cast<QgsVectorLayer*>(l);
        l1->removeSelection();
    }
    this->_mCanvas2D->refresh();
}

void MainWindow::onSendSelected3DLyrEmitted(QString lyrName)
{
    QgsProject* project = QgsProject::instance();
    QVector<QgsVectorLayer*> vecLyrs = project->layers<QgsVectorLayer*>();
    Entity3D entity{};
    for (const auto& vecLyr : vecLyrs) {
        if (vecLyr->name() == lyrName) {
            entity.loadFromVecLyr(*vecLyr);          
        }
    }

    MainWindow3D* mainWindow3D = new MainWindow3D(entity, this->_mAppConfig[MAINWINDOW_3D], this);
    mainWindow3D->show();
}
/*
void MainWindow::onYCZFilterParamsSended(QList<ObPt> obPts, QList<UnobPt> unobPts, QString outputPath)
{
    YCZFilterPyThread* YCZThread = new YCZFilterPyThread(obPts, unobPts, outputPath);
    YCZThread->start();
}

void MainWindow::onOYCZFilterParamsSended(QList<ObPt2D> obpts, QList<Range2D> rangeA, QString outputPath, double s, int k, double c, bool e)
{
    YCZFilterPyThread2D* YCZThread2D = new YCZFilterPyThread2D(obpts, rangeA, outputPath, s, k, c, e);
    YCZThread2D->start();
    
    connect(YCZThread2D, &YCZFilterPyThread2D::escT, this, &MainWindow::onImportResult);
    connect(YCZThread2D, &YCZFilterPyThread2D::_end, YCZThread2D, &YCZFilterPyThread2D::quit);
    
    connect(_mprogress, &yczprogressDialog::canceled, YCZThread2D, &YCZFilterPyThread2D::quit);
}
*/
void MainWindow::onActionHistogram()
{
    QgsProject* project = QgsProject::instance();
    HistogramDialog* Histogram = new HistogramDialog(project, this);
    Histogram->show();
}

void MainWindow::onActionQQPlot()
{
    QgsProject* project = QgsProject::instance();
    QQPlotDialog* QQPlot = new QQPlotDialog(project, this);
    QQPlot->show();
}

void MainWindow::onActionBoxPlot()
{
    QgsProject* project = QgsProject::instance();
    BoxPlotDialog* BoxPlot = new BoxPlotDialog(project, this);
    BoxPlot->show();
}

void MainWindow::onActionLog()
{
    QgsProject* project = QgsProject::instance();
    LogTransDialog* LogTrans = new LogTransDialog(project, this);
    LogTrans->show();
}

void MainWindow::onActionInLog()
{
    QgsProject* project = QgsProject::instance();
    InverseLogDialog* InLogTrans = new InverseLogDialog(project, this);
    InLogTrans->show();
}

void MainWindow::onActionCoAnalyse()
{
    QgsProject* project = QgsProject::instance();
    CoAnalysisDialog* CoAnalyse = new CoAnalysisDialog(project, this);

    CoAnalyse->show();
}

void MainWindow::onActionPanTriggered()
{
    _mCanvas2D->setMapTool(_mPanTool);
}

void MainWindow::onActionZoomInTriggered()
{
    _mCanvas2D->setMapTool(_mZooomInTool);
}

void MainWindow::onActionZoomOutTriggered()
{
    _mCanvas2D->setMapTool(_mZoomOutTool);
}

void MainWindow::onActionZoomFullTriggered()
{
    _mCanvas2D->zoomToFullExtent();
}

void MainWindow::onActionUnsetTriggered()
{
    QgsMapTool* lastMapTool = _mCanvas2D->mapTool();
    _mCanvas2D->unsetMapTool(lastMapTool);
}

void MainWindow::onActionShowTableTriggered()
{
    QModelIndex idx = this->_mLayerTreeView->currentIndex();
    QgsLayerTreeNode* node = this->_mLayerTreeView->index2node(idx);
    if (QgsLayerTree::isLayer(node))
    {
        QgsMapLayer* layer = QgsLayerTree::toLayer(node)->layer();
        if (layer && dynamic_cast<QgsVectorLayer*>(layer)) {
            QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>(layer);

            QgsVectorLayerCache* lc = new QgsVectorLayerCache(vectorLayer, vectorLayer->featureCount());
            QgsAttributeTableView* tv = new QgsAttributeTableView();
            QIcon windowIcon = this->windowIcon();
            tv->setWindowIcon(windowIcon);
            QgsAttributeTableModel* tm = new QgsAttributeTableModel(lc);

            // 加载图层数据
            tm->loadLayer();

            QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel(this->_mCanvas2D, tm, tm);
            tfm->setFilterMode(QgsAttributeTableFilterModel::ShowAll);
            tv->setModel(tfm);
            tv->setWindowModality(Qt::WindowModality::WindowModal);
            tv->setWindowFlag(Qt::Window);
            tv->setWindowTitle("LayerTable");
            tv->setSelectionBehavior(QAbstractItemView::SelectRows);

            // 连接destroyed信号到清理槽函数，使用Qt::DirectConnection确保立即执行
            connect(tv, &QObject::destroyed, lc, &QObject::deleteLater, Qt::DirectConnection);
            connect(tv, &QObject::destroyed, tm, &QObject::deleteLater, Qt::DirectConnection);
            connect(tv, &QObject::destroyed, tfm, &QObject::deleteLater, Qt::DirectConnection);

            // 添加断言检查指针是否有效
            Q_ASSERT(lc != nullptr);
            Q_ASSERT(tm != nullptr);
            Q_ASSERT(tfm != nullptr);
            Q_ASSERT(tv != nullptr);

            tv->show();
        }
        else {
            QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
        }
    }
    else {
        QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
    }
}


void MainWindow::onActionFeatureInfo()
{
    QModelIndex idx = this->_mLayerTreeView->currentIndex();
    QgsLayerTreeNode* node = this->_mLayerTreeView->index2node(idx);
    if (QgsLayerTree::isLayer(node))
    {
        QgsMapLayer* layer = QgsLayerTree::toLayer(node)->layer();
        if (layer && dynamic_cast<QgsVectorLayer*>(layer)) 
        {
            QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>(layer);
            this->_mToolIdentifyFeature = new QgsMapToolIdentifyFeature(this->_mCanvas2D);
            this->_mToolIdentifyFeature->setLayer(vectorLayer);
            QObject::connect(this->_mToolIdentifyFeature, static_cast<void (QgsMapToolIdentifyFeature::*)(const QgsFeature&)>
                (&QgsMapToolIdentifyFeature::featureIdentified), [=](const QgsFeature& feature) {
                onActionClearHighlight();

                this->_mHighlight = new QgsHighlight(_mCanvas2D, feature.geometry(), vectorLayer);
                // DEFAULT_HIGHLIGHT_COLOR = QColor( 255, 0, 0, 128 )
                QColor color = QColor(Qgis::DEFAULT_HIGHLIGHT_COLOR.name());
                color.setAlpha(Qgis::DEFAULT_HIGHLIGHT_COLOR.alpha());
                this->_mHighlight->setColor(color);	
                this->_mHighlight->setFillColor(color);	
                // DEFAULT_HIGHLIGHT_BUFFER_MM = 0.5
                this->_mHighlight->setBuffer(Qgis::DEFAULT_HIGHLIGHT_BUFFER_MM);	
                // DEFAULT_HIGHLIGHT_MIN_WIDTH_MM = 1.0
                this->_mHighlight->setMinWidth(Qgis::DEFAULT_HIGHLIGHT_MIN_WIDTH_MM);
                QString msg;
                QgsFields fields = feature.fields();	
                for (int i = 0; i < fields.count(); i++)
                {
                    msg += QString("%1: %2\n").arg(fields.at(i).name(), feature.attribute(i).toString());
                }
                QMessageBox::information(this, "QgsMapToolIdentifyFeature", msg);
                });
            this->_mCanvas2D->setMapTool(_mToolIdentifyFeature);
        }
        else
        {
            QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
        }
    }
    else {
        QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
    }
}

void MainWindow::onActionEmitPoint()
{
    this->_mToolEmitPoint = new QgsMapToolEmitPoint(this->_mCanvas2D);
    QObject::connect(_mToolEmitPoint, &QgsMapToolEmitPoint::canvasClicked, [=](const QgsPointXY& point, Qt::MouseButton button) {
        QMessageBox::information(this, "QgsMapToolEmitPoint", QString("X: %1\nY: %2").arg(QString::number(point.x()), QString::number(point.y())));
        });
    this->_mCanvas2D->setMapTool(_mToolEmitPoint);
}


void MainWindow::onActionSelectPoint()
{
    QModelIndex idx = this->_mLayerTreeView->currentIndex();
    QgsLayerTreeNode* node = this->_mLayerTreeView->index2node(idx);
    if (QgsLayerTree::isLayer(node))
    {
        QgsMapLayer* layer = QgsLayerTree::toLayer(node)->layer();
        if (layer && dynamic_cast<QgsVectorLayer*>(layer))
        {
            QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>(layer);
            this->_mToolIdentifyFeature = new QgsMapToolIdentifyFeature(this->_mCanvas2D);
            this->_mToolIdentifyFeature->setLayer(vectorLayer);
            QObject::connect(this->_mToolIdentifyFeature, static_cast<void (QgsMapToolIdentifyFeature::*)(const QgsFeature&)>
                (&QgsMapToolIdentifyFeature::featureIdentified), [=](const QgsFeature& feature) {
                    //onActionClearHighlight();

                    this->_mHighlight = new QgsHighlight(_mCanvas2D, feature.geometry(), vectorLayer);
                    // DEFAULT_HIGHLIGHT_COLOR = QColor( 255, 0, 0, 128 )
                    QColor color = QColor(Qgis::DEFAULT_HIGHLIGHT_COLOR.name());
                    color.setAlpha(Qgis::DEFAULT_HIGHLIGHT_COLOR.alpha());
                    this->_mHighlight->setColor(color);
                    this->_mHighlight->setFillColor(color);
                    // DEFAULT_HIGHLIGHT_BUFFER_MM = 0.5
                    this->_mHighlight->setBuffer(Qgis::DEFAULT_HIGHLIGHT_BUFFER_MM);
                    // DEFAULT_HIGHLIGHT_MIN_WIDTH_MM = 1.0
                    this->_mHighlight->setMinWidth(Qgis::DEFAULT_HIGHLIGHT_MIN_WIDTH_MM);
                });
            this->_mCanvas2D->setMapTool(_mToolIdentifyFeature);
        }
        else
        {
            QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
        }
    }
    else {
        QMessageBox::information(this, tr("Information"), tr("NO LAYER IS SELECTED"));
    }
}

void MainWindow::onActionSelectVector()
{

}

void MainWindow::PythonInit()
{
    if (!Py_IsInitialized())
    {
        //1.初始化Python解释器，这是调用操作的第一步
        Py_SetPythonHome((wchar_t*)L"../x64/Release/python"); //TODO: 打包时改为相对路径
        Py_Initialize();
        if (!Py_IsInitialized()) {
            QMessageBox::critical(nullptr, "Error about python.exe", "Fail to initialize python!");
            //return;
        }
        else {
            PyRun_SimpleString("import sys");
            //PyRun_SimpleString("sys.path.append(r'./python')");
            PyRun_SimpleString("sys.path.append(r'../x64/Release/python')");
            PyRun_SimpleString("sys.path.append(r'../x64/Release/plugins')");

            PyEval_InitThreads();
            // 启动子线程前执行，为了释放PyEval_InitThreads获得的全局锁，否则子线程可能无法获取到全局锁。
            PyEval_ReleaseThread(PyThreadState_Get());
            qDebug("Initial Python Success!");
        }
    }
}

bool MainWindow::windowModified()
{
    if (!isWindowModified())
    {
        return true;
    }

    switch (QMessageBox::information(this, QString(), tr("Save current project?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
    {
    case  QMessageBox::Ok:
        saveProject();
        break;
    case  QMessageBox::No:
        break;
    case  QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

bool MainWindow::saveProject()
{
    if (Project::instance()->file().isEmpty())
    {
        const QString& fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), QString(), QString("YangCZ Software Project File(*.ycz)"));

        if (fileName.isEmpty())
        {
            return false;
        }

        QFileInfo fileInfo(fileName);
        Project::instance()->setFile(fileName);
        Project::instance()->setName(fileInfo.baseName());
    }

    if (!Project::instance()->write())
    {
        return false;
    }

    return true;
}

void MainWindow::closeProject()
{
    Project::instance()->reset();
}

void MainWindow::setWindowTitle()
{
    QString projectName = Project::instance()->name();

    if (projectName.isEmpty())
    {
        projectName = tr("Untitled Project");
    }

    QMainWindow::setWindowTitle(tr("[*]%1 - YangCZ Software").arg(projectName));
}

void MainWindow::onActionSerchSQL()
{
    SQLDialog* SQL = new SQLDialog();
}

void MainWindow::openProject(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists() || fileInfo.suffix() != "ycz")
    {
        QMessageBox::information(this, tr("Information"), tr("The project file is not a valid file."));
        //addLog(tr("The project file is not a valid file."));
        return;
    }

    closeProject();
    Project::instance()->setFile(fileName);
    Project::instance()->setName(fileInfo.baseName());

    if (Project::instance()->read())
    {
        //QMessageBox::information(this, tr("Information"), tr("Open project successfully."));
        //addLog(tr("Open project successfully."));
        this->m_workLabel->setText("Open project successfully.");
    }
    else
    {
        QMessageBox::information(this, tr("Information"), tr("Failed to open project."));
        //addLog(tr("Failed to open project."));
    }

    setWindowTitle();
    setWindowModified(false);
}

bool MainWindow::onActionExit()
{
    onActionNew();
    this->close();
    onCanvasRefresh();
    return true;
}

void MainWindow::onMapRefresh()
{
    _mCanvas2D->setLayers(Project::instance()->layerTree()->checkedLayers());
    _mCanvas2D->refresh();
}

void MainWindow::onGetPath(QString path,bool is)
{
    this->resultPath = path;
    this->isTIF = is;
    //_mprogress->show();
}

void MainWindow::onNewProgress(QString name)
{
    _mprogress = new yczprogressDialog(this);
    _mprogress->setWindowTitle(name);
    _mprogress->show();
    _mprogress->onMessage(name + " interpolation is running.");
}

void MainWindow::onWaiting()
{
    QMessageBox::information(this, tr("Waiting"), tr(" This function is still exploiting, please waiting."));
}

void MainWindow::onActionOYangCZ()
{
    QgsProject* project = QgsProject::instance();
    OYangCZServiceDialog* OYangCZ = new OYangCZServiceDialog(project, this);
    OYangCZ->show();
    _moyangcz = OYangCZ;
    connect(OYangCZ, &OYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(OYangCZ, &OYangCZServiceDialog::sendPyParams, this, &MainWindow::onOYangCZParamsSended);

    //QString path = oyczFilterDialog->getOutPath();
    connect(OYangCZ, &OYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onOYangCZParamsSended(QList<double>observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> un_obpts, double c, int k, int dim, QString outputPath, bool e, bool isTIF)
{
    YangCZPyThread* OYangCZThread = new YangCZPyThread(observed_data, obpts, un_obpts, c, k, dim, outputPath, e, isTIF);
    OYangCZThread->start();

    connect(OYangCZThread, &YangCZPyThread::escT, this, &MainWindow::onImportResult);
    connect(OYangCZThread, &YangCZPyThread::_end, OYangCZThread, &YangCZPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, OYangCZThread, &YangCZPyThread::quit);
}

void MainWindow::onNewOYangCZ()
{
    _moyangcz->show();

    connect(_moyangcz, &OYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(_moyangcz, &OYangCZServiceDialog::sendPyParams, this, &MainWindow::onOYangCZParamsSended);

    //QString path = oyczFilterDialog->getOutPath();
    connect(_moyangcz, &OYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onActionYangCZF()
{
    QgsProject* project = QgsProject::instance();
    YangCZFServiceDialog* YangCZF = new YangCZFServiceDialog(project, this);
    YangCZF->show();
    _myangczf = YangCZF;
    connect(YangCZF, &YangCZFServiceDialog::begin, this, &MainWindow::onNewProgress);
    connect(YangCZF, &YangCZFServiceDialog::sendPyParams, this, &MainWindow::onYangCZFParamsSended);
}

void MainWindow::onYangCZFParamsSended(QList<double> observed_data, QList<ObPtXYZ> obpts, double c, int k, int dim)
{
    YangCZFPyThread* YangCZFThread = new YangCZFPyThread(observed_data, obpts, c, k, dim);
    YangCZFThread->start();

    connect(YangCZFThread, &YangCZFPyThread::escT, this, &MainWindow::onEndProgress);
    connect(YangCZFThread, &YangCZFPyThread::_end, YangCZFThread, &YangCZFPyThread::quit);
    connect(_mprogress, &yczprogressDialog::canceled, YangCZFThread, &YangCZFPyThread::quit);
}

void MainWindow::onNewYangCZF()
{
    _myangczf->show();
    connect(_myangczf, &YangCZFServiceDialog::begin, this, &MainWindow::onNewProgress);
    connect(_myangczf, &YangCZFServiceDialog::sendPyParams, this, &MainWindow::onYangCZFParamsSended);
}

void MainWindow::onActionGYangCZ()
{
    QgsProject* project = QgsProject::instance();
    GYangCZServiceDialog* GYangCZ = new GYangCZServiceDialog(project, this);
    GYangCZ->show();
    _mgyangcz = GYangCZ;
    connect(GYangCZ, &GYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(GYangCZ, &GYangCZServiceDialog::sendPyParams, this, &MainWindow::onGYangCZParamsSended);

    //QString path = oyczFilterDialog->getOutPath();
    connect(GYangCZ, &GYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onGYangCZParamsSended(QList<double> observed_data, QList<ObPtXYZ> obpts, QList<double> train_index_matrix, QList<double> test_index_matrix, QList<ObPtXYZ> un_obpts, double c, int k, int k2, int dim, QString outputPath, bool e, bool isTIF)
{
    GYangCZPyThread* GYangCZThread = new GYangCZPyThread(observed_data, obpts, train_index_matrix, test_index_matrix, un_obpts, c, k, k2, dim, outputPath, e, isTIF);
    GYangCZThread->start();

    connect(GYangCZThread, &GYangCZPyThread::escT, this, &MainWindow::onImportResult);
    connect(GYangCZThread, &GYangCZPyThread::_end, GYangCZThread, &GYangCZPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, GYangCZThread, &GYangCZPyThread::quit);
}

void MainWindow::onNewGYangCZ()
{
    _mgyangcz->show();

    connect(_mgyangcz, &GYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(_mgyangcz, &GYangCZServiceDialog::sendPyParams, this, &MainWindow::onGYangCZParamsSended);

    //QString path = oyczFilterDialog->getOutPath();
    connect(_mgyangcz, &GYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}


void MainWindow::onActionSTYangCZ()
{
    QgsProject* project = QgsProject::instance();
    STYangCZServiceDialog* STYangCZ = new STYangCZServiceDialog(project, this);
    STYangCZ->show();
    connect(STYangCZ, &STYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(STYangCZ, &STYangCZServiceDialog::sendPyParams, this, &MainWindow::onSTYangCZParamsSended);

    connect(STYangCZ, &STYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onSTYangCZParamsSended(QList<QList<double>> observed_data, QList<ObPtXYZ> obpts, double c_s, double c_t, int k_s, int k_t, int dim, QString outputPath, bool e)
{
    STYangCZPyThread* STYangCZThread = new STYangCZPyThread(observed_data, obpts, c_s, c_t, k_s, k_t, dim, outputPath, e);
    STYangCZThread->start();

    connect(STYangCZThread, &STYangCZPyThread::escT, this, &MainWindow::onImportResult);
    connect(STYangCZThread, &STYangCZPyThread::_end, STYangCZThread, &STYangCZPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, STYangCZThread, &STYangCZPyThread::quit);
}


void MainWindow::onActionCoYangCZ()
{
    QgsProject* project = QgsProject::instance();
    CoYangCZServiceDialog* CoYangCZ = new CoYangCZServiceDialog(project, this);
    CoYangCZ->show();
    connect(CoYangCZ, &CoYangCZServiceDialog::begin, this, &MainWindow::onNewProgress);
    connect(CoYangCZ, &CoYangCZServiceDialog::sendPyParams, this, &MainWindow::onCoYangCZParamsSended);
    connect(CoYangCZ, &CoYangCZServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onCoYangCZParamsSended(QList<double> observed_data, QList<ObPtXYZ> co_observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> co_obpts, QList<ObPtXYZ> un_obpts, QList<double> c_list, int k, int dim, int n_co, QString outputPath, bool isnugget, bool isTIF)
{
    CoYangCZPyThread* CoYangCZThread = new CoYangCZPyThread(observed_data, co_observed_data, obpts, co_obpts, un_obpts, c_list, k, dim, n_co, outputPath, isnugget, isTIF);
    CoYangCZThread->start();
    connect(CoYangCZThread, &CoYangCZPyThread::escT, this, &MainWindow::onImportResult);
    connect(CoYangCZThread, &CoYangCZPyThread::_end, CoYangCZThread, &CoYangCZPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, CoYangCZThread, &CoYangCZPyThread::quit);
}



void MainWindow::onActionAmoebaG()
{
    QgsProject* project = QgsProject::instance();
    AmoebaGServiceDialog* AmoebaG = new AmoebaGServiceDialog(project, this);
    AmoebaG->show();
    connect(AmoebaG, &AmoebaGServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(AmoebaG, &AmoebaGServiceDialog::sendPyParams, this, &MainWindow::onAmoebaGParamsSended);

    connect(AmoebaG, &AmoebaGServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onAmoebaGParamsSended(QList<double> observed_data, QList<ObPtXYZ> obpts, double length, QString neiPath, int id_sep, bool is_nei, QString outputPath, int dim, double Significant, int repeat_num)
{
    AmoebaGPyThread* AmobeaGThread = new AmoebaGPyThread(observed_data, obpts, length, neiPath, id_sep, is_nei, outputPath, dim, Significant, repeat_num);
    AmobeaGThread->start();

    connect(AmobeaGThread, &STYangCZPyThread::escT, this, &MainWindow::onImportResult);
    connect(AmobeaGThread, &STYangCZPyThread::_end, AmobeaGThread, &AmoebaGPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, AmobeaGThread, &AmoebaGPyThread::quit);
}

void MainWindow::onActionHTMSCAN()
{
    QgsProject* project = QgsProject::instance();
    HTMSCANServiceDialog* HTMSACN = new HTMSCANServiceDialog(project, this);
    HTMSACN->show();
    connect(HTMSACN, &HTMSCANServiceDialog::begin, this, &MainWindow::onNewProgress);

    connect(HTMSACN, &HTMSCANServiceDialog::sendPyParams, this, &MainWindow::onHTMSCANParamsSended);

    connect(HTMSACN, &HTMSCANServiceDialog::getOutPath, this, &MainWindow::onGetPath);
}

void MainWindow::onHTMSCANParamsSended(QList<double> observed_data, QList<ObPtXYZ> obpts, double length, QString neiPath, int id_sep, bool is_nei, QString outputPath, int dim, double Significant, int repeat_num)
{
    HTMSCANPyThread* HTMSCANThread = new HTMSCANPyThread(observed_data, obpts, length, neiPath, id_sep, is_nei, outputPath, dim, Significant, repeat_num);
    HTMSCANThread->start();

    connect(HTMSCANThread, &HTMSCANPyThread::escT, this, &MainWindow::onImportResult);
    connect(HTMSCANThread, &HTMSCANPyThread::_end, HTMSCANThread, &HTMSCANPyThread::quit);

    connect(_mprogress, &yczprogressDialog::canceled, HTMSCANThread, &HTMSCANPyThread::quit);
}


void MainWindow::onImportResult(PyObject* result_re)
{
    int if_run = 2;
    if (result_re != nullptr)
    {
        long longValue = PyLong_AsLong(result_re);
        if_run = static_cast<int>(longValue);
    }

    //if_run = PyArg_Parse(result_re, "i", if_run);
    if (if_run == 1)
    {
        _mprogress->onMessage("YangCZ is successful.");
        _mprogress->onSuccess();
        _moyangcz = nullptr;
        switch (QMessageBox::information(this, QString(), tr("Loading the result?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
        {
        case  QMessageBox::Ok:
            if (this->isTIF) {
                addRasterLayer(this->resultPath);
                this->resultPath = "";
                this->isTIF = false;
                break;
            }
            else {
                LoadingXYZDataDialog* loadingXYZDialog = new LoadingXYZDataDialog(resultPath, Project::instance(), this);
                loadingXYZDialog->show();
                _mCanvas2D->refresh();
                _mEagleEyeMapOverview->refresh();
                this->resultPath = "";
            }
            
        case  QMessageBox::No:
            break;
        case  QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    else if(if_run == 2)
    {
        _mprogress->onMessage("YangCZ interpolation stop running.");
        _mprogress->close();
        switch (QMessageBox::information(this, QString(), tr("Adjust the parameters and try again ?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
        {
        case  QMessageBox::Ok:
            onNewOYangCZ();
            break;
        case  QMessageBox::No:
            break;
        case  QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    else
    {
        _mprogress->onError("error");
        QMessageBox::information(this, tr("Wronging"), tr(" Something was wrong,please check."));
    }
        
    return ;
}

void MainWindow::onEndProgress(PyObject* result_re)
{
    int if_run = 2;
    if (result_re != nullptr)
    {
        long longValue = PyLong_AsLong(result_re);
        if_run = static_cast<int>(longValue);
    }

    //if_run = PyArg_Parse(result_re, "i", if_run);
    if (if_run == 1)
    {
        _mprogress->onMessage("YangCZ is successful.");
        _mprogress->onSuccess();
        _myangczf = nullptr;
    }
    else if (if_run == 2)
    {
        _mprogress->onMessage("YangCZ interpolation stop running.");
        _mprogress->close();
        switch (QMessageBox::information(this, QString(), tr("Adjust the parameters and try again ?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
        {
        case  QMessageBox::Ok:
            onNewYangCZF();
            break;
        case  QMessageBox::No:
            break;
        case  QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    else
    {
        _mprogress->onError("error");
        QMessageBox::information(this, tr("Wronging"), tr(" Something was wrong,please check."));
    }

    return;
}

void MainWindow::onImportResultG(PyObject* result_re)
{
    int if_run = 2;
    if (result_re != nullptr)
    {
        long longValue = PyLong_AsLong(result_re);
        if_run = static_cast<int>(longValue);
    }

    //if_run = PyArg_Parse(result_re, "i", if_run);
    if (if_run == 1)
    {
        _mprogress->onMessage("YangCZ is successful.");
        _mprogress->onSuccess();
        _mgyangcz = nullptr;
        switch (QMessageBox::information(this, QString(), tr("Loading the result?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
        {
        case  QMessageBox::Ok:
            if (this->isTIF) {
                addRasterLayer(this->resultPath);
                this->resultPath = "";
                this->isTIF = false;
                break;
            }
            else {
                LoadingXYZDataDialog* loadingXYZDialog = new LoadingXYZDataDialog(resultPath, Project::instance(), this);
                loadingXYZDialog->show();
                _mCanvas2D->refresh();
                _mEagleEyeMapOverview->refresh();
                this->resultPath = "";
            }

        case  QMessageBox::No:
            break;
        case  QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    else if (if_run == 2)
    {
        _mprogress->onMessage("YangCZ interpolation stop running.");
        _mprogress->close();
        switch (QMessageBox::information(this, QString(), tr("Adjust the parameters and try again ?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
        {
        case  QMessageBox::Ok:
            onNewGYangCZ();
            break;
        case  QMessageBox::No:
            break;
        case  QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    else
    {
        _mprogress->onError("error");
        QMessageBox::information(this, tr("Wronging"), tr(" Something was wrong,please check."));
    }

    return;
}

void MainWindow::onCanvasRefresh()
{
    QList<QgsMapLayer*> mlayers = _mCanvas2D->layers();
    _mEagleEyeMapOverview->setLayers(mlayers);
    _mEagleEyeMapOverview->refresh();
}


void MainWindow::onClickSQLSearch()
{/*
    // 获取输入的查询语句
    QString whereClause = ui.actionSerchSQL->text();
    // 设置的同时会进行查询，如果查询出现错误则返回false
    if (!mpVectorLayer->setSubsetString(whereClause))
    {
        // 如果有查询出现错误
        if (mpVectorLayer->dataProvider()->hasErrors())
        {
            // 显示记录的错误信息
            QMessageBox::warning(this, u8"错误", QString(u8"查询执行错误。返回信息：\n\n%1").arg(mpVectorLayer->dataProvider()->errors().join("\n")));
            // 清除记录的错误信息
            mpVectorLayer->dataProvider()->clearErrors();
        }
        // 如果查询字符串设置失败，而且没有错误，说明设置过滤表达式失败
        else
        {
            QMessageBox::warning(this, u8"错误", u8"设置过滤表达式失败。");
        }
    }*/
}


//void MainWindow::onActionMeasureTriggered()
//{
//}



void MainWindow::onSendSelected3DPathEmitted(QString path)
{
    /*MainWindow3D* mainWindow3D = new MainWindow3D(this->_mAppConfig[MAINWINDOW_3D], this);
    mainWindow3D->show();*/
}

void MainWindow::createStatusBar()
{
    statusBar()->setStyleSheet("QStatusBar::item {border: none;}");

    QFont myFont("Arial", 9);
    statusBar()->setFont(myFont);

    m_workLabel = new QLabel(QString(), statusBar());
    m_workLabel->setObjectName("m_workLabel");
    m_workLabel->setFont(myFont);
    m_workLabel->setMargin(3);
    m_workLabel->setAlignment(Qt::AlignLeft);
    m_workLabel->setFrameStyle(QFrame::NoFrame);
    m_workLabel->setText(tr("YangCZ Software is working."));
    m_workLabel->setToolTip(tr("Working status"));

    // 设置一个固定的宽度，根据需要调整值
    m_workLabel->setFixedWidth(200); // 200 是一个示例值，请根据实际情况调整

    /*
    //! 添加进度条
    m_progressBar = new QProgressBar(statusBar());
    m_progressBar->setObjectName("m_progressBar");
    m_progressBar->setMaximum(100);
    m_progressBar->hide();
    statusBar()->addPermanentWidget(m_progressBar, 1);
    //connect(this->_mCanvas2D, SIGNAL(renderStarting()), this, SLOT(canvasRefreshStarted()));
    //connect(this->_mCanvas2D, SIGNAL(mapCanvasRefreshed()), this, SLOT(canvasRefreshFinished()));

    QFont myFont("Arial", 9);
    statusBar()->setFont(myFont);

    //! 添加坐标显示标签
    m_coordsLabel = new QLabel(QString(), statusBar());
    m_coordsLabel->setObjectName("m_coordsLabel");
    m_coordsLabel->setFont(myFont);
    m_coordsLabel->setMinimumWidth(10);
    m_coordsLabel->setMargin(3);
    m_coordsLabel->setAlignment(Qt::AlignCenter);
    m_coordsLabel->setFrameStyle(QFrame::NoFrame);
    m_coordsLabel->setText(tr("Coordinate:"));
    m_coordsLabel->setToolTip(tr("Current map coordinate"));
    statusBar()->addPermanentWidget(m_coordsLabel, 0);

    //! 坐标编辑标签
    m_coordsEdit = new QLineEdit(QString(), statusBar());
    m_coordsEdit->setObjectName("m_coordsEdit");
    m_coordsEdit->setFont(myFont);
    m_coordsEdit->setMinimumWidth(10);
    m_coordsEdit->setMaximumWidth(300);
    m_coordsEdit->setContentsMargins(0, 0, 0, 0);
    m_coordsEdit->setAlignment(Qt::AlignCenter);
    statusBar()->addPermanentWidget(m_coordsEdit, 0);
    //connect(m_coordsEdit, SIGNAL(returnPressed()), this, SLOT(userCenter()));

    //! 比例尺标签
    m_scaleLabel = new QLabel(QString(), statusBar());
    m_scaleLabel->setObjectName("m_scaleLabel");
    m_scaleLabel->setFont(myFont);
    m_scaleLabel->setMinimumWidth(10);
    m_scaleLabel->setMargin(3);
    m_scaleLabel->setAlignment(Qt::AlignCenter);
    m_scaleLabel->setFrameStyle(QFrame::NoFrame);
    m_scaleLabel->setText(tr("Scale"));
    m_scaleLabel->setToolTip(tr("Current map scale"));
    //statusBar()->addPermanentWidget(m_scaleLabel, 0);

    m_scaleEdit = new QgsScaleComboBox(statusBar());
    m_scaleEdit->setObjectName("m_scaleEdit");
    m_scaleEdit->setFont(myFont);
    m_scaleEdit->setMinimumWidth(10);
    m_scaleEdit->setMaximumWidth(100);
    m_scaleEdit->setContentsMargins(0, 0, 0, 0);
    m_scaleEdit->setToolTip(tr("Current map scale (formatted as x:y)"));
    statusBar()->addPermanentWidget(m_scaleEdit, 0);
    //connect(m_scaleEdit, SIGNAL(scaleChanged()), this, SLOT(userScale()));
    */
}





