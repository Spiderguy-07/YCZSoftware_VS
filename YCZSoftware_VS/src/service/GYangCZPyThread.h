#pragma once
#include <QObject>
#include <QList>
#include <qgsvectorlayer.h>

#include "RunPyService.h"
#include "YangCZPyThread.h"


class GYangCZPyThread : public RunPyService
{
	Q_OBJECT

private:
	QList<double> _mobserved_data;
	QList<ObPtXYZ> _mobpts;
	QList<double> _mtrain_m;
	QList<double> _mtest_m;
	QList<ObPtXYZ> _mun_obpts;
	QString _mOutPath;
	int _mk_num;
	int _mk2_num;
	double _mc_val;
	int _mdim;
	bool _merror_1;
	bool _misTIF;

	PyObject* getObPtXYZList(QList<ObPtXYZ>);
	PyObject* getObPtValList(QList<double>);

protected:
	void setParams() override;
public:
	GYangCZPyThread(QList<double>observed_data, QList<ObPtXYZ> obpts, QList<double>train_index_matrix, QList<double>test_index_matrix, QList<ObPtXYZ> un_obpts, double c, int k, int k2, int dim, QString outputPath, bool e, bool isTIF);
	~GYangCZPyThread();
};