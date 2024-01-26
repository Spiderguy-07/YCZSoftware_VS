#include "RunPyService.h"
#include "../../pythreadstatelock.h"

#include <QMessageBox>
#include <iostream>

RunPyService::RunPyService()
{
    _mPyFile = "";
    _mPyFunc = "";
    _mParams = nullptr;
}

RunPyService::RunPyService(QString pyFile, QString pyFunc, PyObject* params)
{
    _mPyFile = pyFile;
    _mPyFunc = pyFunc;
    _mParams = params;
}

PyObject* RunPyService::callPyFunc(QString pyFile, QString pyFunc, PyObject* params)
{
    
    std::cout << pyFile.toStdString().c_str() ;
    PyObject* module = PyImport_ImportModule(pyFile.toStdString().c_str());
    if (!module) {
        QMessageBox::critical(nullptr, "Error about python.exe", "Fail to import python file!");
        return nullptr;
    }

    PyObject* func = PyObject_GetAttrString(module, pyFunc.toStdString().c_str());
    if (!func) {
        QMessageBox::critical(nullptr, "Error about python.exe", "Fail to import python function!");
        return nullptr;
    }

    PyObject * result =  PyObject_CallFunction(func, "O", params);
    Py_DECREF(func);
    Py_DECREF(module);

    return result;
}

void RunPyService::run()
{
    
    //Py_SetPythonHome((wchar_t*)L"../x64/Release/python"); //TODO: 打包时改为相对路径
    //Py_Initialize();
    //if (!Py_IsInitialized()) {
    //    QMessageBox::critical(nullptr, "Error about python.exe", "Fail to initialize python!");
    //    return;
    //}
    //PyRun_SimpleString("import sys");
    //PyRun_SimpleString("sys.path.append(r'../x64/Release/python')");
    //PyRun_SimpleString("sys.path.append(r'../x64/Release/plugins')"); 
    class PyThreadStateLock PyThreadLock;
    setParams();
    if (_mPyFile.isEmpty() || _mPyFunc.isEmpty()) {
        return;
    }
    if (!_mParams) {
        QMessageBox::critical(nullptr, "Error about python", "Fail to setup parameters!");
        //Py_Finalize();
        return;
    }

    PyObject* final = callPyFunc(_mPyFile, _mPyFunc, _mParams);

    Py_DECREF(_mParams);
    //Py_Finalize();
    emit escT(final);
    emit _end();
}

void RunPyService::setParams()
{
    _mParams = nullptr;
}
