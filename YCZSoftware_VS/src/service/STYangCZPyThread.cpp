#include "STYangCZPyThread.h"


STYangCZPyThread::STYangCZPyThread(QList<QList<double>> observed_data, QList<ObPtXYZ> obpts, double c_s, double c_t, int k_s, int k_t, int dim, QString outputPath, bool e)
{
	_mobserved_data = observed_data;
	_mobpts = obpts;
	_mcs_val = c_s;
	_mct_val = c_t;
	_mks_num = k_s;
	_mkt_num = k_t;
	_mdim = dim;
	_mOutPath = outputPath;
	_merror_1 = e;
}

STYangCZPyThread::~STYangCZPyThread()
{
}

PyObject* STYangCZPyThread::getObPtXYZList(QList<ObPtXYZ> xyz)
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

PyObject* STYangCZPyThread::getObPtValList(QList<QList<double>> vals) {
	PyObject* pyObFeatList = PyList_New(vals.length()); // 创建主列表
	for (int i = 0; i < vals.length(); i++) {
		QList<double> subList = vals.at(i);
		PyObject* pySubList = PyList_New(subList.length()); // 创建子列表

		for (int j = 0; j < subList.length(); j++) {
			double pt = subList.at(j);
			PyObject* obFeat = Py_BuildValue("d", pt); // 创建 double 类型的 PyObject
			PyList_SetItem(pySubList, j, obFeat); // 将 double PyObject 添加到子列表中
			// 不需要 Py_DECREF(obFeat)，因为 PyList_SetItem 会“偷”这个引用
		}

		PyList_SetItem(pyObFeatList, i, pySubList); // 将子列表添加到主列表中
		// 不需要 Py_DECREF(pySubList)，因为 PyList_SetItem 会“偷”这个引用
	}
	return pyObFeatList;
}


void STYangCZPyThread::setParams()
{
	PyObject* obptvalList = getObPtValList(_mobserved_data);
	PyObject* obptsList = getObPtXYZList(_mobpts);

	_mParams = PyTuple_New(9);
	PyTuple_SetItem(_mParams, 0, Py_BuildValue("O", obptvalList));
	PyTuple_SetItem(_mParams, 1, Py_BuildValue("O", obptsList));
	PyTuple_SetItem(_mParams, 2, Py_BuildValue("d", _mcs_val));
	PyTuple_SetItem(_mParams, 3, Py_BuildValue("d", _mct_val));
	PyTuple_SetItem(_mParams, 4, Py_BuildValue("i", _mks_num));
	PyTuple_SetItem(_mParams, 5, Py_BuildValue("i", _mkt_num));
	PyTuple_SetItem(_mParams, 6, Py_BuildValue("i", _mdim));
	PyTuple_SetItem(_mParams, 7, Py_BuildValue("s", _mOutPath.toStdString().c_str()));
	PyTuple_SetItem(_mParams, 8, Py_BuildValue("b", _merror_1));

	Py_DECREF(obptvalList);
	Py_DECREF(obptsList);
	_mPyFile = "STYangCZ";
	_mPyFunc = "run_for_soft";
}