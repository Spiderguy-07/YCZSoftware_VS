#include "Entity3D.h"

#include <qgswkbtypes.h>
#include <QMessageBox>

Entity3D::Entity3D()
{
	_mGeometryType = Entity3D::GeometryType::UnKnown;
	_mMinX = DBL_MAX;
	_mMinY = DBL_MAX;
	_mMinZ = DBL_MAX;
	_mMaxX = -DBL_MAX;
	_mMaxY = -DBL_MAX;
	_mMaxZ = -DBL_MAX;
	
}


void Entity3D::loadFromVecLyr(const QgsVectorLayer& vecLyr)
{
	
	Qgis::WkbType geoType = vecLyr.wkbType();
	switch (geoType) {
	case Qgis::WkbType::PointZ:
		_mGeometryType = Entity3D::GeometryType::Point;
		convertPoint(vecLyr);
		break;
	case Qgis::WkbType::MultiPointZ:
		break;
	default:
		break;

	}
}

void Entity3D::convertPoint(const QgsVectorLayer& vecLyr)
{
	QgsFeatureIterator iter = vecLyr.getFeatures();
	QgsFeature feat;
	uint count = 0;
	

	// 使用 vecLyr 的字段来初始化这些字典
	const QgsFields fields = vecLyr.fields();
	for (int i = 0; i < fields.count(); ++i) {
		minValues[fields.at(i).name()] = QVariant::fromValue(std::numeric_limits<double>::max());
		maxValues[fields.at(i).name()] = QVariant::fromValue(std::numeric_limits<double>::lowest());
	}

	while (iter.nextFeature(feat)) {
		QgsGeometry geo = feat.geometry();
		// 准备一个 QVariantMap 来存储属性键值对
		QVariantMap attributesMap;

		// 获取字段名称列表
		const QgsFields fields = vecLyr.fields();
		for (int i = 0; i < fields.count(); ++i) {
			// 使用字段名称作为键，属性值作为值
			//attributesMap[fields.at(i).name()] = feat.attribute(i);

			QString fieldName = fields.at(i).name();
			QVariant fieldValue = feat.attribute(i);

			// 使用字段名称作为键，属性值作为值
			attributesMap[fieldName] = fieldValue;
			bool ok;
			double value = fieldValue.toDouble(&ok);
			if (ok) {
				if (value < minValues[fieldName].toDouble()) {
					minValues[fieldName] = value;
				}
				if (value > maxValues[fieldName].toDouble()) {
					maxValues[fieldName] = value;
				}
			}
		}

		for (auto it = geo.vertices_begin(); it != geo.vertices_end(); it++) {
			QgsPoint coor = QgsPoint(*it);
			double x = coor.x();
			double y = coor.y();
			double z = coor.z();
			_mMinX = x < _mMinX ? x : _mMinX;
			_mMinY = y < _mMinY ? y : _mMinY;
			_mMinZ = z < _mMinZ ? z : _mMinZ;
			_mMaxX = x > _mMaxX ? x : _mMaxX;
			_mMaxY = y > _mMaxY ? y : _mMaxY;
			_mMaxZ = z > _mMaxZ ? z : _mMaxZ;
			
			_mVecX.push_back(x);
			_mVecY.push_back(y);
			_mVecZ.push_back(z);
			_mIndices.push_back(count);
			for (int j = 0; j < 36; j++)
			{
				_mCubeIndices.push_back(count * 36 + j);
			}
			_mAttributes.push_back(attributesMap);
			count++;
		}	
	}

	
}

Entity3D::GeometryType Entity3D::getGeometryType()
{
	return _mGeometryType;
}

void Entity3D::setNormalizeVer(double centerX, double centerY, double centerZ, double scale)
{
	if (_mVertices.isEmpty()) {
		for (int i = 0; i < _mVecX.size(); i++) {
			double x = _mVecX.at(i);
			double y = _mVecY.at(i);
			double z = _mVecZ.at(i);
			x = (x - centerX) * scale;
			y = (y - centerY) * scale;
			z = (z - centerZ) * scale;
			Vertex ver;
			ver.position = QVector3D(x, y, z);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);
			_mVertices.push_back(ver);
		}
	}
	else {
		for (int i = 0; i < _mVecX.size(); i++) {
			double x = _mVecX.at(i);
			double y = _mVecY.at(i);
			double z = _mVecZ.at(i);
			x = (x - centerX) * scale;
			y = (y - centerY) * scale;
			z = (z - centerZ) * scale;
			Vertex ver = _mVertices.at(i);
			ver.position = QVector3D(x, y, z);
			_mVertices.replace(i, ver);
		}
	}
}

void Entity3D::setVerticesColor(const QVector4D& newColor)
{
	for (int i = 0; i < _mVertices.size(); ++i) {
		Vertex& ver = _mVertices[i]; 
		ver.color = newColor; 
	}
	for (int i = 0; i < _mCubeVertices.size(); ++i) {
		Vertex& ver = _mCubeVertices[i];
		ver.color = newColor;
	}
}

void Entity3D::setOneColor(int index, const QVector4D& newColor)
{
	Vertex& ver = _mVertices[index];
	ver.color = newColor;
	for (int i = 0; i < 36; i++)
	{
		Vertex& ver = _mCubeVertices[index*36+i];
		ver.color = newColor;
	}
}



QVector<Vertex> Entity3D::getNormalizeVer()
{
	return _mVertices;
}

QVector<uint> Entity3D::getIndices()
{
	return _mIndices;
}

QVector<Vertex> Entity3D::getCubeNormalizeVer()
{
	return _mCubeVertices;
}

QVector<uint> Entity3D::getCubeIndices()
{
	return _mCubeIndices;
}

double Entity3D::getMinX()
{
	return _mMinX;
}

void Entity3D::setCubeNormalizeVer(double _mCenterX, double _mCenterY, double _mCenterZ, double _mScale, double cube_size)
{
	double halfSide = cube_size / 2.0;
	if (_mCubeVertices.isEmpty()) {
		for (int i = 0; i < _mVecX.size(); i++) {
			double x = _mVecX.at(i);
			double y = _mVecY.at(i);
			double z = _mVecZ.at(i);

			double vertices[][3] = {
				{(x - halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front bottom left
				{(x + halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front bottom right
				{(x + halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front top right
				{(x - halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front top left
				{(x - halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back bottom left
				{(x + halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back bottom right
				{(x + halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back top right
				{(x - halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}  // Back top left
			};

			//front face
			double x_p = vertices[0][0];
			double y_p = vertices[0][1];
			double z_p = vertices[0][2];

			Vertex ver;

			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			//back face
			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			//top face
			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			//bottom face
			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];

			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			//left face
			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];

			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			//right face
			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			// TODO: get default color from configuration file
			ver.color = QVector4D(0.0f, 1.0f, 0.0f, 1.0f);

			_mCubeVertices.push_back(ver);
		}
	}
	else {
		for (int i = 0; i < _mVecX.size(); i++) {
			double x = _mVecX.at(i);
			double y = _mVecY.at(i);
			double z = _mVecZ.at(i);

			double vertices[][3] = {
				{(x - halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front bottom left
				{(x + halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front bottom right
				{(x + halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front top right
				{(x - halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z + halfSide - _mCenterZ) * _mScale}, // Front top left
				{(x - halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back bottom left
				{(x + halfSide - _mCenterX) * _mScale, (y - halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back bottom right
				{(x + halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}, // Back top right
				{(x - halfSide - _mCenterX) * _mScale, (y + halfSide - _mCenterY) * _mScale, (z - halfSide - _mCenterZ) * _mScale}  // Back top left
			};

			//front face
			double x_p = vertices[0][0];
			double y_p = vertices[0][1];
			double z_p = vertices[0][2];

			Vertex ver;

			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 0].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+0, ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 1].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+1, ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 2].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+2, ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 3].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+3, ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 4].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+4, ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 5].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+5, ver);

			//back face
			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 6].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+6, ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 7].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+7, ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 8].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+8, ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 9].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+9, ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 10].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+10, ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 11].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+11, ver);

			//top face
			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 12].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+12, ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 13].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+13, ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 14].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+14, ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 15].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+15, ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 16].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+16, ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 17].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+17, ver);

			//bottom face
			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 18].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+18, ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 19].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+19, ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 20].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+20, ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 21].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+21, ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 22].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+22, ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];

			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 23].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+23, ver);

			//left face
			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 24].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+24, ver);

			x_p = vertices[0][0];
			y_p = vertices[0][1];
			z_p = vertices[0][2];

			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 25].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+25, ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 26].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+26, ver);

			x_p = vertices[4][0];
			y_p = vertices[4][1];
			z_p = vertices[4][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 27].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+27, ver);

			x_p = vertices[3][0];
			y_p = vertices[3][1];
			z_p = vertices[3][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 28].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+28, ver);

			x_p = vertices[7][0];
			y_p = vertices[7][1];
			z_p = vertices[7][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 29].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+29, ver);

			//right face
			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 30].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+30, ver);

			x_p = vertices[5][0];
			y_p = vertices[5][1];
			z_p = vertices[5][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 31].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+31, ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 32].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+32, ver);

			x_p = vertices[1][0];
			y_p = vertices[1][1];
			z_p = vertices[1][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 33].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+33, ver);

			x_p = vertices[6][0];
			y_p = vertices[6][1];
			z_p = vertices[6][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 + 34].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+34, ver);

			x_p = vertices[2][0];
			y_p = vertices[2][1];
			z_p = vertices[2][2];
			ver.position = QVector3D(x_p, y_p, z_p);
			ver.color = _mCubeVertices[i * 36 +35].color;
			// TODO: get default color from configuration file

			_mCubeVertices.replace(i*36+35, ver);
		}
	}
}

double Entity3D::getMinY()
{
	return _mMinY;
}

double Entity3D::getMinZ()
{
	return _mMinZ;
}

double Entity3D::getMaxX()
{
	return _mMaxX;
}

double Entity3D::getMaxY()
{
	return _mMaxY;
}

double Entity3D::getMaxZ()
{
	return _mMaxZ;
}

int Entity3D::getVerticesCount()
{
	return _mVertices.count();
}

int Entity3D::getCubeVerticesCount()
{
	return _mCubeVertices.count();
}

double Entity3D::getMinValue(QString key)
{
	return minValues[key].toDouble();
}

int Entity3D::getIndicesCount()
{
	return _mIndices.count();
}

int Entity3D::getCubeIndicesCount()
{
	return _mCubeIndices.count();
}

double Entity3D::getMaxValue(QString key)
{
	return maxValues[key].toDouble();
}

QStringList Entity3D::getFileds()
{
	QStringList fields;
	for (auto it = minValues.begin(); it != minValues.end(); ++it) {
		fields.append(it.key());
	}
	return fields;
}

double Entity3D::getValue(int place, QString key)
{
	return  _mAttributes[place][key].toDouble();
}
/*
QVector<double> Entity3D::getXYZ(int index_v)
{
	QVector<double> xyz = {};
	xyz.push_back(_mVecX[index_v]);
	xyz.push_back(_mVecX[index_v]);
	xyz.push_back(_mVecX[index_v]);
	return xyz;
}
*/
void Entity3D::setGeometryType(int index)
{
	if (index == 0)
	{
		_mGeometryType = Entity3D::GeometryType::UnKnown;
	}
	else if (index == 1)
	{
		_mGeometryType = Entity3D::GeometryType::Point;
	}
	else if (index == 2)
	{
		_mGeometryType = Entity3D::GeometryType::Line;
	}
	else if (index == 3)
	{
		_mGeometryType = Entity3D::GeometryType::Polygon;
	}
	else if (index == 4)
	{
		_mGeometryType = Entity3D::GeometryType::Cube;
	}
	else
	{
	}

}



