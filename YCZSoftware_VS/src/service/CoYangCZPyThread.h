#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"
#include "YangCZPyThread.h"
class CoYangCZPyThread : public RunPyService
{
	Q_OBJECT

private:
	QList<double> _mobserved_data;
	QList<ObPtXYZ> _mco_observed_data;
	QList<ObPtXYZ> _mobpts;
	QList<ObPtXYZ> _mco_obpts;
	QList<ObPtXYZ> _mun_obpts;
	QList<double> _mc_list;
	QString _mOutPath;
	int _mnum_co;
	int _mk_num;
	int _mdim;
	bool _mis_nug;
	bool _misTif;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<double>);

protected:
	void setParams() override;

public:
	CoYangCZPyThread(QList<double> observed_data, QList<ObPtXYZ> co_observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> co_obpts, QList<ObPtXYZ> un_obpts, QList<double> c_list, int k, int dim, int n_co, QString outputPath, bool isnugget, bool isTIF);
	~CoYangCZPyThread();
};

