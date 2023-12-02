#include "MainWindow3D.h"
#include <QMessageBox>
#include "../macro.h"

MainWindow3D::MainWindow3D(Entity3D entity3d, Json::Value config, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->_mConfig = config;
	this->entities.push_back(entity3d);
	this->initUi();
}

MainWindow3D::~MainWindow3D()
{}


void MainWindow3D::initUi()
{
	// set window size
	this->setGeometry(this->_mConfig[X_LOCATION].asInt(),
		this->_mConfig[Y_LOCATION].asInt(),
		this->_mConfig[WIDTH].asInt(),
		this->_mConfig[HEIGHT].asInt());

	this->setWindowTitle(this->_mConfig[TITLE].asCString());  // set title

	// create left dock
	this->_mLayerDock = new QgsDockWidget(this);
	this->_mLayerDock->setWindowTitle(this->_mConfig[LAYER_DOCK_3D_TITLE].asCString());
	this->_mLayerDock->setObjectName(this->_mConfig[LAYER_DOCK_3D_OBJ_NAME].asCString());
	this->addDockWidget(Qt::LeftDockWidgetArea, this->_mLayerDock);
	
	// create container of 3D canvas
	this->_mCenterTabWidget = new QTabWidget(this);

	// create 3D canvas
	Json::Value canvas3DConfig = this->_mConfig[OSG_WIDGET];
	this->_mCanvas3D = new Canvas3D(this->entities[0], this);
	
	// add 3D canvas to container
	this->_mCenterTabWidget->addTab(this->_mCanvas3D, this->_mConfig[MAP_VIEW_TITLE].asCString());
	this->setCentralWidget(this->_mCenterTabWidget);

	this->connect(ui.actionImport3D, &QAction::triggered, this, &MainWindow3D::onActionImport3DTriggered);
}

bool MainWindow3D::onActionImport3DTriggered()
{
	QStringList lyrFileList = FileLoadingService::open3DFileSelectDialog(this->_mConfig[OPEN_VEC_3D_DIALOG_TITLE].asCString(),
		this->_mConfig[VEC_3D_FILTER_NAME].asCString(), this);
	if (lyrFileList.size() <= 0) return false;
	for (QString path : lyrFileList) {
		path = path.trimmed();
		//this->_mCanvas3D->add3DObj(path);
	}
	return true;
}
