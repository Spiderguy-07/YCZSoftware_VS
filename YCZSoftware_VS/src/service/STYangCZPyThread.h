#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"
#include "YangCZPyThread.h"

class STYangCZPyThread : public RunPyService
{
	Q_OBJECT

private:
	QList<QList<double>> _mobserved_data;
	QList<ObPtXYZ> _mobpts;
	QString _mOutPath;
	int _mks_num;
	int _mkt_num;
	double _mcs_val;
	double _mct_val;
	int _mdim;
	bool _merror_1;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<QList<double>>);

protected:
	void setParams() override;
public:
	STYangCZPyThread(QList<QList<double>>observed_data, QList<ObPtXYZ> obpts, double c_s, double c_t, int k_s, int k_t, int dim, QString outputPath, bool e);
	~STYangCZPyThread();
};

