#pragma once

#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"

struct ObPt2D {
	double x;
	double y;
	double val;

	ObPt2D(double x, double y, double val) {
		this->x = x;
		this->y = y;
		this->val = val;
	}
};

struct Range2D {
	double x;
	double y;

	Range2D(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

class YCZFilterPyThread2D : public RunPyService
{
	Q_OBJECT

private:
	QList<ObPt2D> _mObPts2D;
	QList<Range2D> _mRange2D;
	QString _mOutPath;
	double _msize;
	int _mk_num;
	double _mc_val;
	bool _merror_1;

	PyObject* getObPt2DList();
	PyObject* getRange2DList();

protected:
	void setParams() override;

public:
	YCZFilterPyThread2D(QList<ObPt2D> obPts, QList<Range2D> rangeA, QString outPath, double s, int k, double c, bool e);
	~YCZFilterPyThread2D();
};
