#pragma once

#include <qgsvectorlayer.h>
#include <QVector4D>


struct Vertex {
	QVector3D position;
	QVector4D color;
};


class Entity3D
{

public:
	enum GeometryType {
		UnKnown = 0,
		Point = 1,
		Line = 2,
		Polygon = 3
	};
	Entity3D();

	void loadFromVecLyr(const QgsVectorLayer& vecLyr);
	void convertPoint(const QgsVectorLayer& vecLyr);


	GeometryType getGeometryType();

	void setNormalizeVer(double centerX, double centerY, double centerZ, double scale);

	QVector<Vertex> getNormalizeVer();
	QVector<uint> getIndices();
	double getMinX();
	double getMinY();
	double getMinZ();
	double getMaxX();
	double getMaxY();
	double getMaxZ();

	int getVerticesCount();
	int getIndicesCount();

private:
	QVector<Vertex> _mVertices;
	QVector<uint> _mIndices;
	QVector<double> _mVecX;
	QVector<double> _mVecY;
	QVector<double> _mVecZ;

	double _mMinX;
	double _mMinY;
	double _mMinZ;
	double _mMaxX;
	double _mMaxY;
	double _mMaxZ;

	
	GeometryType _mGeometryType;

};