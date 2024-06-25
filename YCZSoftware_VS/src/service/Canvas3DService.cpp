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
	entity.setCubeNormalizeVer(_mCenterX, _mCenterY, _mCenterZ, _mScale, 2.0);
	entity.setVerticesColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
	_mEntities.push_back(entity);
	// TODO: get translate and rotate speed from configuration file
	_mTransSpeed = 0.05f;
	_mRotSpeed = M_PI / 180.0f / 3.0f;
	_mZoomSpeed = 0.015f;
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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);// background color
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
	_mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../YCZSoftware_VS/resource/shader/gl.vert");
	_mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../YCZSoftware_VS/resource/shader/gl.frag");
	_mShaderProgram.link();

	

	Entity3D firstEntity = _mEntities.at(0);
	QVector<Vertex> vertices = firstEntity.getNormalizeVer();
	QVector<uint> indices = firstEntity.getIndices();



	int verNum = vertices.count();

	//std::cout << vertices.data();
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

	std::shared_ptr<QOpenGLVertexArrayObject> cubeVAO{ new QOpenGLVertexArrayObject{} };
	QOpenGLBuffer cubeVBO{ QOpenGLBuffer::VertexBuffer };
	QOpenGLBuffer cubeEBO{ QOpenGLBuffer::IndexBuffer };
	_mVAOs.push_back(cubeVAO);
	_mVBOs.push_back(cubeVBO);
	_mEBOs.push_back(cubeEBO);
	cubeVAO->create();
	cubeVAO->bind();
	cubeVBO.create();
	cubeVBO.bind();
	cubeEBO.create();
	cubeEBO.bind();

	QVector<Vertex> cubevertices = firstEntity.getCubeNormalizeVer();
	QVector<uint> cubeindices = firstEntity.getCubeIndices();

	/*
	// create shader
	_mCubeShaderProgram.create();
	_mCubeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../YCZSoftware_VS/resource/shader/gl.vert");
	_mCubeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../YCZSoftware_VS/resource/shader/gl.frag");
	_mCubeShaderProgram.link();*/
	_mShaderProgram.enableAttributeArray(0);
	_mShaderProgram.setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
	_mShaderProgram.enableAttributeArray(1);
	_mShaderProgram.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, color), 4, sizeof(Vertex));

	int verCubeNum = cubevertices.count();

	cubeEBO.allocate(cubeindices.data(), sizeof(uint) * cubeindices.count());
	cubeVBO.allocate(cubevertices.data(), sizeof(Vertex) * verCubeNum);

	/*
	// bind VBO to VAO
	_mCubeShaderProgram.enableAttributeArray(0);
	_mCubeShaderProgram.setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
	_mCubeShaderProgram.enableAttributeArray(1);
	_mCubeShaderProgram.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, color), 4, sizeof(Vertex));*/

	cubeVAO->release();
	cubeVBO.release();
	cubeEBO.release();
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

	/*
	//cube shader
	_mCubeShaderProgram.bind();

	_mCubeShaderProgram.setUniformValue("model", model);

	_mCubeShaderProgram.setUniformValue("view", view);

	_mCubeShaderProgram.setUniformValue("projection", projection);*/

	// render models
	int count = 0;
	Entity3D entity = _mEntities.at(count);
	switch (entity.getGeometryType()) {
	case Entity3D::Point:
	{
		const auto& VAO = _mVAOs[0];
		QOpenGLVertexArrayObject::Binder bind(&*VAO);

		glDrawElements(GL_POINTS, entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
		break;
	}
	case Entity3D::Polygon:
	{
		const auto& VAO = _mVAOs[0];
		QOpenGLVertexArrayObject::Binder bind(&*VAO);
		glDrawElements(GL_TRIANGLES, entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
		break;
	}	
	case Entity3D::Cube:
	{
		//_mEntities[0].setCubeNormalizeVer(_mCenterX, _mCenterY, _mCenterZ, _mScale, 10.0);

		QVector<Vertex> cubevertices = _mEntities[0].getCubeNormalizeVer();

		_mVBOs[1].bind();

		_mVBOs[1].allocate(cubevertices.constData(), cubevertices.size() * sizeof(Vertex));

		_mVBOs[1].release();

		const auto& VAO = _mVAOs[1];
		QOpenGLVertexArrayObject::Binder bind(&*VAO);
		glDrawElements(GL_TRIANGLES, entity.getCubeIndicesCount(), GL_UNSIGNED_INT, nullptr);
		break;
	}	
	default:
		break;
	}
	/*
	int count = 0;
	for (const auto &VAO : _mVAOs) {
		QOpenGLVertexArrayObject::Binder bind(&*VAO);
		Entity3D entity = _mEntities.at(count);
		
		switch (entity.getGeometryType()) {
		case Entity3D::Point:
			glDrawElements(GL_POINTS , entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
			break;
		case Entity3D::Polygon:
			glDrawElements(GL_TRIANGLES, entity.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
			break;
		case Entity3D::Cube:
			glDrawElements(GL_TRIANGLES, entity.getCubeIndicesCount(), GL_UNSIGNED_INT, nullptr);
			
			break;
		default:
			break;
		}
		// For each vertex in the entity, draw a sphere

		count++;
	}	*/

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

	//QVector4D redColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色，不透明

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

void Canvas3D::changColor(QVector4D newColor)
{
	//QVector4D newColor(1.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < _mEntities.size(); ++i) {
		_mEntities[i].setVerticesColor(newColor);

		QVector<Vertex> vertices = _mEntities[i].getNormalizeVer();

		_mVBOs[0].bind();

		_mVBOs[0].allocate(vertices.constData(), vertices.size() * sizeof(Vertex));

		_mVBOs[0].release();

		/*
		QVector<Vertex> cubevertices = _mEntities[i].getCubeNormalizeVer();

		_mVBOs[1].bind();

		_mVBOs[1].allocate(cubevertices.constData(), cubevertices.size() * sizeof(Vertex));

		_mVBOs[1].release();*/
	}
	update();
}

void Canvas3D::gradeColor(QVector4D startColor, QVector4D endColor, int steps, QString filed)
{
	//double minValue = 

	for (int i = 0; i < _mEntities.size(); ++i) {

		int NumVertices = _mEntities[i].getVerticesCount();

		double MinV = _mEntities[i].getMinValue(filed);
		double MaxV = _mEntities[i].getMaxValue(filed);

		double long_s = (MaxV - MinV) / steps;


		float H_s, S_s, L_s;
		RGBToHSL(startColor, H_s, S_s, L_s);
		float H_e, S_e, L_e;
		RGBToHSL(endColor, H_e, S_e, L_e);
		float step1 = (H_e - H_s) / (steps - 1);
		float step2 = (S_e - S_s) / (steps - 1);
		float step3 = (L_e - L_s) / (steps - 1);

		for (int j = 0; j < NumVertices; ++j) {
			double Value = _mEntities[i].getValue(j, filed);

			int i_color = (Value - MinV) / long_s;
			if (Value == MaxV) {
				i_color = i_color - 1;
			}
			/*
			float factor = static_cast<float>(i_color) / (steps - 1);
			
			QVector4D interpolatedColor = startColor * (1.0f - factor) + endColor * factor;*/


			float H_n = H_s + step1 * i_color;
			float S_n = S_s + step2 * i_color;
			float L_n = L_s + step3 * i_color;

			QVector4D interpolatedColor = HSLToRGB(H_n, S_n, L_n, 1.0f);
			_mEntities[i].setOneColor(j, interpolatedColor);
		}


		QVector<Vertex> vertices = _mEntities[i].getNormalizeVer();

		_mVBOs[0].bind();

		_mVBOs[0].allocate(vertices.constData(), vertices.size() * sizeof(Vertex));

		_mVBOs[0].release();


		QVector<Vertex> cubevertices = _mEntities[i].getCubeNormalizeVer();

		_mVBOs[1].bind();

		_mVBOs[1].allocate(cubevertices.constData(), cubevertices.size() * sizeof(Vertex));

		_mVBOs[1].release();
	}
	update();
}

void Canvas3D::cubeDraw(int index, double size)
{
	for (int i = 0; i < _mEntities.size(); ++i) {
		_mEntities[i].setGeometryType(index);
	}
	_mSizeCube = size;
	_mEntities[0].setCubeNormalizeVer(_mCenterX, _mCenterY, _mCenterZ, _mScale, _mSizeCube);
	

	/*
	QVector<Vertex> cubevertices = _mEntities[0].getCubeNormalizeVer();

	_mVBOs[1].bind();

	_mVBOs[1].allocate(cubevertices.constData(), cubevertices.size() * sizeof(Vertex));

	_mVAOs[1]->bind(); 

	_mCubeShaderProgram.enableAttributeArray(0);
	_mCubeShaderProgram.setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
	_mCubeShaderProgram.enableAttributeArray(1);
	_mCubeShaderProgram.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, color), 4, sizeof(Vertex));


	_mVBOs[1].release();
	_mVAOs[1]->release();*/
	update();
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

QVector4D Canvas3D::HSLToRGB(float Hue, float Saturation, float Lightness, float alpha) {
	Saturation /= 100.0f;  // 假设Saturation和Lightness现在是百分比形式
	Lightness /= 100.0f;

	float C = (1 - std::abs(2 * Lightness - 1)) * Saturation;
	float hh = Hue / 60.0;
	float xiaoshu = hh - std::floor(hh);
	float X = C * (1 - std::abs(std::fmod(hh, 2) - 1));
	float r = 0, g = 0, b = 0;
	if (hh >= 0 && hh < 1) {
		r = C; g = X;
	}
	else if (hh >= 1 && hh < 2) {
		r = X; g = C;
	}
	else if (hh >= 2 && hh < 3) {
		g = C; b = X;
	}
	else if (hh >= 3 && hh < 4) {
		g = X; b = C;
	}
	else if (hh >= 4 && hh < 5) {
		r = X; b = C;
	}
	else if (hh >= 5 && hh < 6) {
		r = C; b = X;
	}
	float m = Lightness - C / 2;
	r += m; g += m; b += m;

	return QVector4D(r, g, b, alpha);
}

void Canvas3D::RGBToHSL(const QVector4D& color, float& H, float& S, float& L) {
	float r = color.x();
	float g = color.y();
	float b = color.z();
	float max_rgb = std::max({ r, g, b });
	float min_rgb = std::min({ r, g, b });
	float d = max_rgb - min_rgb;

	if (d == 0) H = 0;
	else if (max_rgb == r) H = std::fmod((g - b) / d, 6);
	else if (max_rgb == g) H = (b - r) / d + 2;
	else H = (r - g) / d + 4;

	H *= 60;
	if (H < 0) H += 360;

	L = (max_rgb + min_rgb) / 2;
	S = (d == 0) ? 0 : d / (1 - std::abs(2 * L - 1));

	// 转换回百分比形式
	S *= 100;
	L *= 100;
}
