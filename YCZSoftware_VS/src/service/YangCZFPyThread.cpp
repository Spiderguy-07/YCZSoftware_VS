#include "YangCZFPyThread.h"
#include <QMessageBox>

YangCZFPyThread::YangCZFPyThread(QList<double> observed_data, QList<ObPtXYZ> obpts, double c, int k, int dim)
{
	_mobserved_data = observed_data;
	_mobpts = obpts;
	_mc_val = c;
	_mk_num = k;
	_mdim = dim;
}

YangCZFPyThread::~YangCZFPyThread()
{
}

PyObject* YangCZFPyThread::getObPtXYZList(QList<ObPtXYZ> xyz) {
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

PyObject* YangCZFPyThread::getObPtValList(QList<double> vals) {
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

void YangCZFPyThread::setParams()
{
	PyObject* obptvalList = getObPtValList(_mobserved_data);
	PyObject* obptsList = getObPtXYZList(_mobpts);

	_mParams = PyTuple_New(5);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obptvalList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", obptsList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("d", _mc_val));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("i", _mk_num));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("i", _mdim));

	Py_DECREF(obptvalList);
	Py_DECREF(obptsList);
	_mPyFile = "YangCZF";
	_mPyFunc = "run_for_soft";
}
