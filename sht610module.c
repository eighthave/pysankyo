
#include <Python.h>
#include <structmember.h>

#include <stdio.h>

#include "Prtcl_USB_HID_SHT.h"
#include "USB_HID_Def_SHT.h"


typedef struct {
    PyObject_HEAD
    unsigned long productid;  /* USB productID */
    PyObject* serial;         /* USB serial */
    unsigned long timeout;    /* timeout for executing commands in ms */
} CardDispenser;


static void CardDispenser_dealloc(CardDispenser* self)
{
    Py_XDECREF(self->serial);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* CardDispenser_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    CardDispenser *self;

    self = (CardDispenser *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->productid = 0x10aa;
        self->serial = PyString_FromString("");
        if (self->serial == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->timeout = 20000;
    }

    return (PyObject *)self;
}


static PyObject* CardDispenser_init(CardDispenser* self, PyObject* args, PyObject *kwds)
{
    PyObject* serial = NULL;

    static char *kwlist[] = {"productid", "serial", "timeout", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|kOk", kwlist,
                                      &self->productid, &serial, &self->timeout))
        return (PyObject*)-1;
    if (serial) {
        PyObject* tmp = self->serial;
        Py_INCREF(serial);
        self->serial = serial;
        Py_XDECREF(tmp);
    }
    
    printf("self %p\n", self);
    printf("productid: %lx serial: %p timeout: %lu\n",
           self->productid, serial, self->timeout);
    printf("sht610.__init__ called\n");

    return 0;
}

static PyObject* CardDispenser_version(CardDispenser* self, PyObject* args)
{
    printf("sht610.version called\n");
    LIB_INFORMATION_SHT libinfo;
    USB_HID_SHT_GetLibraryRevision(&libinfo);
    return Py_BuildValue("ssss",
                         libinfo.InterfaceLib.Filename,
                         libinfo.InterfaceLib.Revision,
                         libinfo.ProtocolLib.Filename,
                         libinfo.ProtocolLib.Revision);
}

static PyObject* CardDispenser_open(CardDispenser* self, PyObject* args)
{
    printf("sht610.open called\n");
    printf("productid: %lx serial: %p\n", self->productid, self->serial);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_close(CardDispenser* self, PyObject* args)
{
    printf("sht610.close called\n");
//    USB_HID_SHT_DisconnectDevice(c_ulong(self.productid),
//                                 c_char_p(self.currentserial)
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_reset(CardDispenser* self, PyObject* args)
{
    printf("sht610.reset called\n");
    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef CardDispenser_methods[] = 
{
    {"version", (PyCFunction)CardDispenser_version, METH_NOARGS,
	 "return the interface and protocol version information"},
    {"open", (PyCFunction)CardDispenser_open, METH_NOARGS,
	 "open the control connection to the device"},
    {"close", (PyCFunction)CardDispenser_close, METH_NOARGS,
	 "close the connection to the device"},
    {"reset", (PyCFunction)CardDispenser_reset, METH_NOARGS,
	 "reset the device and initialize it"},
    {NULL},
};

static PyMemberDef CardDispenser_members[] = {
    {"productid", T_ULONG, offsetof(CardDispenser, productid), 0,
     "USB productId of targetted device"},
    {"serial", T_OBJECT_EX, offsetof(CardDispenser, serial), 0,
     "USB serial number of targetted device"},
    {"timeout", T_ULONG, offsetof(CardDispenser, timeout), 0,
     "timeout for executing commands in ms"},
    {NULL}  /* Sentinel */
};

static PyTypeObject CardDispenserType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "sht610.CardDispenser",    /*tp_name*/
    sizeof(CardDispenser),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)CardDispenser_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "CardDispenser objects",   /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    CardDispenser_methods,     /* tp_methods */
    CardDispenser_members,     /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)CardDispenser_init,      /* tp_init */
    0,                         /* tp_alloc */
    CardDispenser_new,         /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifdef __cplusplus
extern "C"
#endif
void initsht610(void)
{
    if (PyType_Ready(&CardDispenserType) < 0)
        return;

    /* create a new module and class */
    PyObject* m = Py_InitModule("sht610", module_methods);

    if (m == NULL)
        return;

    Py_INCREF(&CardDispenserType);
    PyModule_AddObject(m, "CardDispenser", (PyObject *)&CardDispenserType);    
}

