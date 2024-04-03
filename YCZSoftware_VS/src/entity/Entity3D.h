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
		Polygon = 3,
		Cube = 4
	};
	Entity3D();

	void loadFromVecLyr(const QgsVectorLayer& vecLyr);
	void convertPoint(const QgsVectorLayer& vecLyr);


	GeometryType getGeometryType();

	void setNormalizeVer(double centerX, double centerY, double centerZ, double scale);
	void setVerticesColor(const QVector4D& newColor);
	void setOneColor(int index, const QVector4D& newColor);

	void setCubeNormalizeVer(double centerX, double centerY, double centerZ, double scale, double cube_size);

	QVector<Vertex> getNormalizeVer();
	QVector<uint> getIndices();

	QVector<Vertex> getCubeNormalizeVer();
	QVector<uint> getCubeIndices();

	double getMinX();
	double getMinY();
	double getMinZ();
	double getMaxX();
	double getMaxY();
	double getMaxZ();

	double getMinValue(QString key);
	double getMaxValue(QString key);

	double getValue(int place, QString key);

	QStringList getFileds();

	int getVerticesCount();
	int getIndicesCount();

	int getCubeVerticesCount();
	int getCubeIndicesCount();

	//QVector<double> getXYZ(int index_v);

	void setGeometryType(int index);

private:
	//QVector4D _mColor;
	QVector<Vertex> _mVertices;
	QVector<uint> _mIndices;

	QVector<Vertex> _mCubeVertices;
	QVector<uint> _mCubeIndices;

	QVector<double> _mVecX;
	QVector<double> _mVecY;
	QVector<double> _mVecZ;
	QVector<QVariantMap> _mAttributes;

	double _mMinX;
	double _mMinY;
	double _mMinZ;
	double _mMaxX;
	double _mMaxY;
	double _mMaxZ;


	QVariantMap minValues;
	QVariantMap maxValues;
	
	GeometryType _mGeometryType;

};