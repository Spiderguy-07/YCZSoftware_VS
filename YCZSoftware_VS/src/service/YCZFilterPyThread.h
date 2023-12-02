#pragma once

#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"

struct ObPt {
	double x;
	double y;
	double z;
	double val;
	int lbl;

	ObPt(double x, double y, double z, double val, int lbl) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->val = val;
		this->lbl = lbl;
	}
};

struct UnobPt {
	double x;
	double y;
	double z;
	int lbl;

	UnobPt(double x, double y, double z, int lbl) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->lbl = lbl;
	}
};

class YCZFilterPyThread  : public RunPyService
{
	Q_OBJECT

private:
	QList<ObPt> _mObPts;
	QList<UnobPt> _mUnobPts;
	QString _mOutPath;

	PyObject* getObPtList();
	PyObject* getUnobPtList();

protected:
	void setParams() override;

public:
	YCZFilterPyThread(QList<ObPt> obPts, QList<UnobPt> unobPts, QString outPath);
	~YCZFilterPyThread();
};
