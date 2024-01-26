#pragma once

#include <Python.h>
#include <QString>
#include <QThread>

class RunPyService : public QThread
{
	Q_OBJECT
public:
	explicit RunPyService();
	explicit RunPyService(QString pyFile, QString pyFunc, PyObject* params);
	static PyObject* callPyFunc(QString pyFile, QString pyFunc, PyObject* params);

protected:
	void run() override;
	virtual void setParams();

	QString _mPyFile;
	QString _mPyFunc;
	PyObject* _mParams;

signals:
	void escT(PyObject* result_re);
	void _end();
};

