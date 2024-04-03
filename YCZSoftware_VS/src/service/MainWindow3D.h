#pragma once

#include <QMainWindow>
#include <qgsdockwidget.h>
#include <json.h>
#include <QColorDialog>

#include "ui_MainWindow3D.h"
#include "Canvas3DService.h"
#include "FileLoadingService.h"
#include "ColorGradientWidget.h"
#include "CubeDialog.h"


class MainWindow3D : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow3D(Entity3D entity3d, Json::Value config, QWidget *parent = nullptr);
	~MainWindow3D();
	

private:
	Ui::MainWindow3DClass ui;
	Json::Value _mConfig;

	QgsDockWidget* _mLayerDock;
	QTabWidget* _mCenterTabWidget;
	Canvas3D* _mCanvas3D;

	QList<Entity3D> entities;
	

	void initUi();
	bool onActionImport3DTriggered();
	void onActionColor();
	void onActionGrade();
	void onActionCube();

	void GradeColor(QColor begin_color, QColor end_color, int steps, QString name);
};
