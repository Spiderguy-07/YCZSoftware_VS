#include "YCZFilterPyThread2D.h"

#include <QMessageBox>


PyObject* YCZFilterPyThread2D::getObPt2DList()
{
	PyObject* pyObFeatList = PyList_New(_mObPts2D.length());
	for (int i = 0; i < _mObPts2D.length(); i++) {
		PyObject* obFeat = PyList_New(3);
		ObPt2D pt = _mObPts2D.at(i);
		PyList_SetItem(obFeat, 0, Py_BuildValue("d", pt.x));
		PyList_SetItem(obFeat, 1, Py_BuildValue("d", pt.y));
		PyList_SetItem(obFeat, 2, Py_BuildValue("d", pt.val));

		PyList_SetItem(pyObFeatList, i, Py_BuildValue("O", obFeat));
		Py_DECREF(obFeat);
	}
	return pyObFeatList;
}

PyObject* YCZFilterPyThread2D::getRange2DList()
{
	PyObject* pyUnobFeatList = PyList_New(_mRange2D.length());
	for (int i = 0; i < _mRange2D.length(); i++) {
		PyObject* unobFeat = PyList_New(2);
		Range2D pt = _mRange2D.at(i);
		PyList_SetItem(unobFeat, 0, Py_BuildValue("d", pt.x));
		PyList_SetItem(unobFeat, 1, Py_BuildValue("d", pt.y));

		PyList_SetItem(pyUnobFeatList, i, Py_BuildValue("O", unobFeat));
		Py_DECREF(unobFeat);
	}
	return pyUnobFeatList;
}

void YCZFilterPyThread2D::setParams()
{
	PyObject* obPt2DList = getObPt2DList();
	PyObject* range2DList = getRange2DList();

	_mParams = PyTuple_New(7);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obPt2DList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", range2DList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("d", _msize));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("i", _mk_num));
	PyTuple_SetItem(_mParams, 5, Py_BuildValue("d", _mc_val));
	PyTuple_SetItem(_mParams, 6, Py_BuildValue("b", _merror_1));

	Py_DECREF(obPt2DList);
	Py_DECREF(range2DList);
	_mPyFile = "OYangCZ";
	_mPyFunc = "run_for_soft";
}

YCZFilterPyThread2D::YCZFilterPyThread2D(QList<ObPt2D> obPts2D, QList<Range2D> rangeA, QString outPath, double s, int k,double c, bool e)
{
	_mObPts2D = obPts2D;
	_mRange2D = rangeA;
	_mOutPath = outPath;
	_msize = s;
	_mk_num = k;
	_mc_val = c;
	_merror_1 = e;
}

YCZFilterPyThread2D::~YCZFilterPyThread2D()
{

}
