#include "HTMSCANPyThread.h"

HTMSCANPyThread::HTMSCANPyThread(QList<double> observed_data, QList<ObPtXYZ> obpts, double length, QString neiPath, int id_sep, bool is_nei, QString outputPath, int dim, double Significant, int repeat_num)
{
	_mobserved_data = observed_data;
	_mobpts = obpts;
	_mlength = length;
	_mNeiPath = neiPath;
	_misNei = is_nei;
	_mid_sep = id_sep;
	_mOutPath = outputPath;
	_mdim = dim;
	_mSignificant = Significant;
	_mrepeat_num = repeat_num;
}

HTMSCANPyThread::~HTMSCANPyThread()
{
}

PyObject* HTMSCANPyThread::getObPtXYZList(QList<ObPtXYZ> xyz)
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

PyObject* HTMSCANPyThread::getObPtValList(QList<double> vals)
{
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

void HTMSCANPyThread::setParams()
{
	PyObject* obptvalList = getObPtValList(_mobserved_data);
	PyObject* obptsList = getObPtXYZList(_mobpts);

	_mParams = PyTuple_New(10);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obptvalList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", obptsList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("d", _mlength));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("s", _mNeiPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("i", _mid_sep));
	PyTuple_SetItem(_mParams, 5, Py_BuildValue("b", _misNei));
	PyTuple_SetItem(_mParams, 6, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 7, Py_BuildValue("i", _mdim));
	PyTuple_SetItem(_mParams, 8, Py_BuildValue("d", _mSignificant));
	PyTuple_SetItem(_mParams, 9, Py_BuildValue("i", _mrepeat_num));

	Py_DECREF(obptvalList);
	Py_DECREF(obptsList);
	_mPyFile = "AMOEBA_S";
	_mPyFunc = "run";
}

