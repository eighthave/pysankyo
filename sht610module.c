
#include <Python.h>

static PyObject* CardDispenser_init(PyObject *self, PyObject *args)
{
    printf("sht610.__init__ called\n");
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_doSomething(PyObject *self, PyObject *args)
{
    printf("sht610.doSomething called\n");
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef sht610Methods[] = 
{
    {"__init__", CardDispenser_init, METH_VARARGS, 
	 "doc string"},
    {"doSomething", CardDispenser_doSomething, METH_VARARGS,
	 "doc string"},
    {NULL},
};

static PyMethodDef ModuleMethods[] = { {NULL} };

#ifdef __cplusplus
extern "C"
#endif
void initsht610()
{
    PyMethodDef *def;

    /* create a new module and class */
    PyObject *module = Py_InitModule("sht610", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("CardDispenser");
    PyObject *sht610Class = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "CardDispenser", sht610Class);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(sht610Class);
    
    /* add methods to class */
    for (def = sht610Methods; def->ml_name != NULL; def++) {
	PyObject *func = PyCFunction_New(def, NULL);
	PyObject *method = PyMethod_New(func, NULL, sht610Class);
	PyDict_SetItemString(classDict, def->ml_name, method);
	Py_DECREF(func);
	Py_DECREF(method);
    }
}

