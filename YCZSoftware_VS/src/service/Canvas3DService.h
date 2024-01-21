#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMouseEvent>

#include "../entity/Entity3D.h"
#include "ui_Canvas3DService.h"

//#define _USE_MATH_DEFINES
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Canvas3DServiceClass; };
QT_END_NAMESPACE


class Canvas3D : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	Canvas3D(Entity3D entity, QWidget *parent = nullptr);
	~Canvas3D();


protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

	void wheelEvent(QWheelEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::Canvas3DServiceClass *ui;
	QOpenGLShaderProgram _mShaderProgram;
	QVector<std::shared_ptr<QOpenGLVertexArrayObject>> _mVAOs;
	QVector<QOpenGLBuffer> _mVBOs;
	QVector<QOpenGLBuffer> _mEBOs;
	QVector<Entity3D> _mEntities;

	QVector3D _mViewPos;
	QVector3D _mViewAt;
	QVector3D _mViewUp;


	double _mAspect;
	double _mFov;
	double _mZNear;
	double _mZFar;

	double _mCenterX;
	double _mCenterY;
	double _mCenterZ;
	double _mScale;

	float _mRotSpeed;
	float _mTransSpeed;
	float _mZoomSpeed;
	float _mRotX;
	float _mRotY;
	float _mZoom;
	float _mTransX;
	float _mTransY;
	

	QPoint _mLastPos;
	float _mLastZoom;

	void setNormalizeParamWithEntity(Entity3D entity);
};
