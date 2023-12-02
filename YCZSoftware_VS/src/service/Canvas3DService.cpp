#include "Canvas3DService.h"

#include <QMessageBox>
#include <QOpenglFunctions_4_5_Core>


Canvas3D::Canvas3D(Entity3D entity, QWidget *parent)
	: QOpenGLWidget(parent)
	, ui(new Ui::Canvas3DServiceClass())
{
	ui->setupUi(this);
	setNormalizeParamWithEntity(entity);
	entity.setNormalizeVer(_mCenterX, _mCenterY, _mCenterZ, _mScale);
	_mEntities.push_back(entity);
	// TODO: get translate and rotate speed from configuration file
	_mTransSpeed = 0.05f;
	_mRotSpeed = M_PI / 180.0f / 3.0f;
	_mZoomSpeed = 0.03f;
	_mRotX = 0.0f;
	_mRotY = 0.0f;
	_mZoom = 1.0f;
	_mTransX = 0.0f;
	_mTransY = 0.0f;
	_mLastZoom = 1.0f;
	_mViewPos = QVector3D{ 0.0f, 0.0f, 3.0f };
	_mViewAt = QVector3D{ 0.0f, 0.0f, 0.0f };
	_mViewUp = QVector3D{ 0.0f, 1.0f, 0.0f };
	_mAspect = this->width() / (double)this->height();
	_mFov = 45;
	_mZNear = 0.001;
	_mZFar = 100;
}

Canvas3D::~Canvas3D()
{
	makeCurrent();
	delete ui;
}

void Canvas3D::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);  // clear color buffer
	glEnable(GL_DEPTH_TEST);
	
	
	// create object in GPU
	std::shared_ptr<QOpenGLVertexArrayObject> VAO{new QOpenGLVertexArrayObject{}};
	QOpenGLBuffer VBO{QOpenGLBuffer::VertexBuffer};
	QOpenGLBuffer EBO{QOpenGLBuffer::IndexBuffer};
	_mVAOs.push_back(VAO);
	_mVBOs.push_back(VBO);
	_mEBOs.push_back(EBO);
	VAO->create();
	VAO->bind();
	VBO.create();
	VBO.bind();
	EBO.create();
	EBO.bind();

	// create shader
	_mShaderProgram.create();
	_mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/YCZSoftware_VS/resource/shader/gl.vert");
	_mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/YCZSoftware_VS/resource/shader/gl.frag");
	_mShaderProgram.link();

	

	Entity3D firstEntity = _mEntities.at(0);
	QVector<Vertex> vertices = firstEntity.getNormalizeVer();
	QVector<uint> indices = firstEntity.getIndices();

	int verNum = vertices.count();

	// apply for space in GPU
	EBO.allocate(indices.data(), sizeof(uint) * indices.count());
	VBO.allocate(vertices.data(), sizeof(Vertex) * verNum);

	// bind VBO to VAO
	_mShaderProgram.enableAttributeArray(0);
	_mShaderProgram.setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
	_mShaderProgram.enableAttributeArray(1);
	_mShaderProgram.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, color), 4, sizeof(Vertex));
	

	VAO->release();
	VBO.release();
	EBO.release();
}

void Canvas3D::paintGL()
{
	_mShaderProgram.bind();
	
	QMatrix4x4 model;
	model.translate(_mTransX, _mTransY, 0.0f);
	model.scale(_mZoom);
	model.rotate(_mRotX, QVector3D(1.0f, 0.0f, 0.0f));
	model.rotate(_mRotY, QVector3D(0.0f, 1.0f, 0.0f));
	_mShaderProgram.setUniformValue("model", model);

	QMatrix4x4 view;
	view.lookAt(_mViewPos, _mViewAt, _mViewUp);
	_mShaderProgram.setUniformValue("view", view);

	QMatrix4x4 projection;
	projection.perspective(_mFov, _mAspect, _mZNear, _mZFar);
	_mShaderProgram.setUniformValue("projection", projection);


	// render models
	int count = 0;
	for (const auto &VAO : _mVAOs) {
		QOpenGLVertexArrayObject::Binder bind(&*VAO);
		Entity3D entity = _mEntities.at(count);
		switch (entity.getGeometryType()) {
		case Entity3D::Point:
			glDrawElements(GL_POINTS, entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
			break;
		case Entity3D::Polygon:
			glDrawElements(GL_TRIANGLES, entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);

			break;
		default:
			break;
		}
		
		count++;
	}
}

void Canvas3D::resizeGL(int w, int h)
{
	_mAspect = h > 0 ? (w / (double)h) : 1.0;
	glViewport(0, 0, w, h);
}

void Canvas3D::wheelEvent(QWheelEvent* event)
{
	int d = event->angleDelta().y();
	if (d > 0) {
		d = d * _mZoomSpeed;
		_mZoom = _mLastZoom * d;
	}
	else if (d < 0){
		d = -d * _mZoomSpeed;
		_mZoom = _mLastZoom / d;
	}
	_mLastZoom = _mZoom;
	update();
}

void Canvas3D::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	float dx = pos.x() - _mLastPos.x();
	float dy = pos.y() - _mLastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		_mRotY += dx * _mRotSpeed;
		_mRotX += dy * _mRotSpeed;
	}
	if (event->buttons() & Qt::RightButton) {
		int width = this->width();
		int height = this->height();

		float rx = _mTransSpeed / (0.5f * width);
		_mTransX += dx * rx;
		float ry = _mTransSpeed / (0.5f * height);
		_mTransY -= dy * ry;
	}
	update();
}

void Canvas3D::mousePressEvent(QMouseEvent* event)
{
	if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::RightButton)) {
		_mLastPos = event->pos();
	}
}

void Canvas3D::setNormalizeParamWithEntity(Entity3D entity)
{
	double minx = entity.getMinX();
	double miny = entity.getMinY();
	double minz = entity.getMinZ();
	double maxx = entity.getMaxX();
	double maxy = entity.getMaxY();
	double maxz = entity.getMaxZ();
	_mCenterX = (maxx + minx) / 2;
	_mCenterY = (maxy + miny) / 2;
	_mCenterZ = (maxz + minz) / 2;
	double halfRangeX = maxx - _mCenterX;
	double halfRangeY = maxy - _mCenterY;
	double halfRangeZ = maxz - _mCenterZ;
	double halfRange = 0;
	if (halfRangeX > halfRangeY && halfRangeX > halfRangeZ) {
		halfRange = halfRangeX;
	}
	else if (halfRangeY > halfRangeZ && halfRangeY > halfRangeX) {
		halfRange = halfRangeY;
	}
	else {
		halfRange = halfRangeZ;
	}
	_mScale = 1 / halfRange;
}
