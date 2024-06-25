#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"
#include "YangCZPyThread.h"

//struct ObPtXYZ {
//	double x;
//	double y;
//	double z;
//	ObPtXYZ(double x, double y, double z) {
//		this->x = x;
//		this->y = y;
//		this->z = z;
//	}
//};
class AmoebaGPyThread : public RunPyService
{
private:
	QList<double> _mobserved_data;
	QList<ObPtXYZ> _mobpts;
	QString _mOutPath;
	QString _mNeiPath;
	int _mid_sep;
	double _mlength;
	int _mdim;
	bool _misNei;
	double _mSignificant;
	int _mrepeat_num;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<double>);

protected:
	void setParams() override;

public:
	AmoebaGPyThread(QList<double>observed_data, QList<ObPtXYZ> obpts, double length, QString neiPath, int id_sep, bool is_nei, QString outputPath, int dim, double Significant, int repeat_num);
	~AmoebaGPyThread();
};

