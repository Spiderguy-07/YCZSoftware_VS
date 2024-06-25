#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"
#include "YangCZPyThread.h"

class YangCZFPyThread : public RunPyService
{
	Q_OBJECT

private:
	QList<double> _mobserved_data;
	QList<ObPtXYZ> _mobpts;
	int _mk_num;
	double _mc_val;
	int _mdim;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<double>);

protected:
	void setParams() override;
public:

	YangCZFPyThread(QList<double> observed_data, QList<ObPtXYZ> obpts, double c, int k, int dim);
	~YangCZFPyThread();
};