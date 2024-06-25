#include "CoYangCZPyThread.h"

CoYangCZPyThread::CoYangCZPyThread(QList<double> observed_data, QList<ObPtXYZ> co_observed_data, QList<ObPtXYZ> obpts, QList<ObPtXYZ> co_obpts, QList<ObPtXYZ> un_obpts, QList<double> c_list, int k, int dim, int n_co, QString outputPath, bool isnugget, bool isTIF)
{
	_mobserved_data = observed_data;
	_mco_observed_data = co_observed_data;
	_mobpts = obpts;
	_mco_obpts = co_obpts;
	_mun_obpts = un_obpts;
	_mc_list = c_list;
	_mk_num = k;
	_mdim = dim;
	_mnum_co = n_co;
	_mOutPath = outputPath;
	_mis_nug = isnugget;
	_misTif = isTIF;
}

CoYangCZPyThread::~CoYangCZPyThread()
{
}

PyObject* CoYangCZPyThread::getObPtXYZList(QList<ObPtXYZ> xyz)
{
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

PyObject* CoYangCZPyThread::getObPtValList(QList<double> vals) {
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

void CoYangCZPyThread::setParams()
{
	PyObject* obptvalList = getObPtValList(_mobserved_data);
	PyObject* co_obptvalList = getObPtXYZList(_mco_observed_data);
	PyObject* obptsList = getObPtXYZList(_mobpts);
	PyObject* co_obptsList = getObPtXYZList(_mco_obpts);
	PyObject* un_obptsList = getObPtXYZList(_mun_obpts);
	PyObject* obc_list = getObPtValList(_mc_list);

	_mParams = PyTuple_New(12);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obptvalList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", co_obptvalList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("O", obptsList));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("O", co_obptsList));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("O", un_obptsList));
	PyTuple_SetItem(_mParams, 5, Py_BuildValue("O", obc_list));
	PyTuple_SetItem(_mParams, 6, Py_BuildValue("i", _mk_num));
	PyTuple_SetItem(_mParams, 7, Py_BuildValue("i", _mdim));
	PyTuple_SetItem(_mParams, 8, Py_BuildValue("i", _mnum_co));
	PyTuple_SetItem(_mParams, 9, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 10, Py_BuildValue("b", _mis_nug));
	PyTuple_SetItem(_mParams, 11, Py_BuildValue("b", _misTif));

	Py_DECREF(obptvalList);
	Py_DECREF(co_obptvalList);
	Py_DECREF(obptsList);
	Py_DECREF(co_obptsList);
	Py_DECREF(un_obptsList);
	Py_DECREF(obc_list);
	_mPyFile = "CoYangCZ";
	_mPyFunc = "run_for_soft";
}
