#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"

struct ObPtXYZ {
	double x;
	double y;
	double z;
	ObPtXYZ(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
class YangCZPyThread : public RunPyService
{
	Q_OBJECT

private:
	QList<double> _mobserved_data;
	QList<ObPtXYZ> _mobpts;
	QList<ObPtXYZ> _mun_obpts;
	QString _mOutPath;
	int _mk_num;
	double _mc_val;
	int _mdim;
	bool _merror_1;
	bool _misTIF;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<double>);

protected:
	void setParams() override;

public:
	YangCZPyThread(QList<double>observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> un_obpts, double c, int k, int dim, QString outputPath, bool e, bool isTIF);
	~YangCZPyThread();
};

