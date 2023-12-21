#include "YCZFilterPyThread.h"

#include <QMessageBox>


PyObject* YCZFilterPyThread::getObPtList()
{
	PyObject* pyObFeatList = PyList_New(_mObPts.length());
	for (int i = 0; i < _mObPts.length(); i++) {
		PyObject* obFeat = PyList_New(5);
		ObPt pt = _mObPts.at(i);
		PyList_SetItem(obFeat, 0, Py_BuildValue("d", pt.x));
		PyList_SetItem(obFeat, 1, Py_BuildValue("d", pt.y));
		PyList_SetItem(obFeat, 2, Py_BuildValue("d", pt.z));
		PyList_SetItem(obFeat, 3, Py_BuildValue("d", pt.val));
		PyList_SetItem(obFeat, 4, Py_BuildValue("i", pt.lbl));

		PyList_SetItem(pyObFeatList, i, Py_BuildValue("O", obFeat));
		Py_DECREF(obFeat);
	}
	return pyObFeatList;
}

PyObject* YCZFilterPyThread::getUnobPtList()
{
	PyObject* pyUnobFeatList = PyList_New(_mUnobPts.length());
	for (int i = 0; i < _mUnobPts.length(); i++) {
		PyObject* unobFeat = PyList_New(4);
		UnobPt pt = _mUnobPts.at(i);
		PyList_SetItem(unobFeat, 0, Py_BuildValue("d", pt.x));
		PyList_SetItem(unobFeat, 1, Py_BuildValue("d", pt.y));
		PyList_SetItem(unobFeat, 2, Py_BuildValue("d", pt.z));
		PyList_SetItem(unobFeat, 3, Py_BuildValue("i", pt.lbl));

		PyList_SetItem(pyUnobFeatList, i, Py_BuildValue("O", unobFeat));
		Py_DECREF(unobFeat);
	}
	return pyUnobFeatList;
}

void YCZFilterPyThread::setParams()
{
	PyObject* obPtList = getObPtList();
	PyObject* unobPtList = getUnobPtList();

	_mParams = PyTuple_New(3);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obPtList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", unobPtList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	Py_DECREF(obPtList);
	Py_DECREF(unobPtList);
	_mPyFile = "GYangCZ";
	_mPyFunc = "run_for_grid";
}

YCZFilterPyThread::YCZFilterPyThread(QList<ObPt> obPts, QList<UnobPt> unobPts, QString outPath)
{
	_mObPts = obPts;
	_mUnobPts = unobPts;
	_mOutPath = outPath;
}

YCZFilterPyThread::~YCZFilterPyThread()
{}
