#include "GYangCZPyThread.h"

GYangCZPyThread::GYangCZPyThread(QList<double> observed_data, QList<ObPtXYZ> obpts, QList<double> train_index_matrix, QList<double> test_index_matrix, QList<ObPtXYZ> un_obpts, double c, int k, int k2, int dim, QString outputPath, bool e, bool isTIF)
{
	_mobserved_data = observed_data;
	_mobpts = obpts;
	_mtrain_m = train_index_matrix;
	_mtest_m = test_index_matrix;
	_mun_obpts = un_obpts;
	_mc_val = c;
	_mk_num = k;
	_mk2_num = k2;
	_mdim = dim;
	_mOutPath = outputPath;
	_merror_1 = e;
	_misTIF = isTIF;
}

GYangCZPyThread::~GYangCZPyThread()
{
}

PyObject* GYangCZPyThread::getObPtXYZList(QList<ObPtXYZ> xyz) {
	PyObject* pyObFeatList = PyList_New(xyz.length());
	for (int i = 0; i < xyz.length(); i++) {
		PyObject* obFeat = PyList_New(3);
		ObPtXYZ pt = xyz.at(i);
		PyList_SetItem(obFeat, 0, Py_BuildValue("d", pt.x));
		PyList_SetItem(obFeat, 1, Py_BuildValue("d", pt.y));
		PyList_SetItem(obFeat, 2, Py_BuildValue("d", pt.z));

		PyList_SetItem(pyObFeatList, i, Py_BuildValue("O", obFeat));
		Py_DECREF(obFeat);
	}
	return pyObFeatList;
}

PyObject* GYangCZPyThread::getObPtValList(QList<double> vals) {
	PyObject* pyObFeatList = PyList_New(vals.length());
	for (int i = 0; i < vals.length(); i++) {
		PyObject* obFeat = PyList_New(1);
		double pt = vals.at(i);
		PyList_SetItem(obFeat, 0, Py_BuildValue("d", pt));

		PyList_SetItem(pyObFeatList, i, Py_BuildValue("O", obFeat));
		Py_DECREF(obFeat);
	}
	return pyObFeatList;
}

void GYangCZPyThread::setParams()
{
	PyObject* obptvalList = getObPtValList(_mobserved_data);
	PyObject* obptsList = getObPtXYZList(_mobpts);
	PyObject* obptsTrain = getObPtValList(_mtrain_m);
	PyObject* obptsTest = getObPtValList(_mtest_m);
	PyObject* un_obptsList = getObPtXYZList(_mun_obpts);

	_mParams = PyTuple_New(12);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obptvalList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", obptsList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("O", obptsTrain));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("O", obptsTest));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("O", un_obptsList));
	PyTuple_SetItem(_mParams, 5, Py_BuildValue("d", _mc_val));
	PyTuple_SetItem(_mParams, 6, Py_BuildValue("i", _mk_num));
	PyTuple_SetItem(_mParams, 7, Py_BuildValue("i", _mk2_num));
	PyTuple_SetItem(_mParams, 8, Py_BuildValue("i", _mdim));
	PyTuple_SetItem(_mParams, 9, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 10, Py_BuildValue("b", _merror_1));
	PyTuple_SetItem(_mParams, 11, Py_BuildValue("b", _misTIF));

	Py_DECREF(obptvalList);
	Py_DECREF(obptsList);
	Py_DECREF(un_obptsList);
	_mPyFile = "GYangCZ";
	_mPyFunc = "run_for_soft";
}