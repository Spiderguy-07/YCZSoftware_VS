#include "Entity3D.h"

#include <qgswkbtypes.h>
#include <QMessageBox>

Entity3D::Entity3D()
{
	_mGeometryType = Entity3D::GeometryType::UnKnown;
	_mMinX = DBL_MAX;
	_mMinY = DBL_MAX;
	_mMinZ = DBL_MAX;
	_mMaxX = DBL_MIN;
	_mMaxY = DBL_MIN;
	_mMaxZ = DBL_MIN;
	
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
	while (iter.nextFeature(feat)) {
		QgsGeometry geo = feat.geometry();
		
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


QVector<Vertex> Entity3D::getNormalizeVer()
{
	return _mVertices;
}

QVector<uint> Entity3D::getIndices()
{
	return _mIndices;
}


double Entity3D::getMinX()
{
	return _mMinX;
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

int Entity3D::getIndicesCount()
{
	return _mIndices.count();
}



