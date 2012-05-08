
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
    printf("sht610.__init__ called\n");
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
    printf("productid: %lx serial: %p '%s' timeout: %lu\n",
           self->productid, serial, PyString_AsString(self->serial), self->timeout);

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
    long ret;
    char currentserial[16];
    char* serial = PyString_AsString(self->serial);
    if(strcmp(serial, "") == 0)
        serial = NULL;

    bzero(currentserial, sizeof(currentserial));

    printf("productid: %lx serial: %p '%s' timeout: %lu\n",
           self->productid, self->serial, serial, self->timeout);
 
    ret = USB_HID_SHT_ConnectDevice(self->productid,
                                    serial,
                                    currentserial);

    if(ret == _USB_HID_SHT_NO_ERROR)
    {
        printf("command finished with no error\n");
    }
    else
    {
        printf("command finished with ERROR: %lx\n", ret);
        Py_INCREF(Py_False);
        return Py_False;
    }

    if(serial == NULL || strcmp(serial, currentserial) != 0)
    {
        PyObject* tmp = self->serial;
        self->serial = PyString_FromString(currentserial);
        Py_XDECREF(tmp);
    }
    printf("productid: %lx serial: %p '%s' timeout: %lu\n",
           self->productid, self->serial, currentserial, self->timeout);

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject* CardDispenser_close(CardDispenser* self, PyObject* args)
{
    printf("sht610.close called\n");
    USB_HID_SHT_DisconnectDevice(self->productid,
                                 PyString_AsString(self->serial));
    Py_INCREF(Py_None);
    return Py_None;
}

static long execute_command(CardDispenser* self,
                            unsigned char command, unsigned char parameter,
                            unsigned char* data, unsigned long size)
{
    long ret;
    COMMAND_SHT command_sht;                // command message to send
    REPLY_SHT reply;                    // reply message to receive
    //unsigned char data[] = { 0x30, 0x30, 0x30,}; //init command data

    command_sht.CommandCode = command;    //init command
    command_sht.ParameterCode = parameter;
    command_sht.Data.pBody = data;        // start address of region where data is stored
    command_sht.Data.Size = size;         // data size in bytes

// Executes Initialize command, and then receives a reply for the command
    printf("Executing 'initialize' command...\n");
    ret = USB_HID_SHT_ExecuteCommand(self->productid,
                                     PyString_AsString(self->serial),
                                     &command_sht,
                                     self->timeout,
                                     &reply);
    if(ret == _USB_HID_SHT_NO_ERROR)
    {
        printf("command finished with no error\n");
        // init command successfully finished, now parse reply
        if( reply.replyType == PositiveReply )
        {
            printf("received positive reply\n");
        }
        else if( reply.replyType == NegativeReply )
        {
            printf("received negative reply\n");
        }
        else
        {
            printf("received other reply\n");
        }
        return 1;
    }
    else
    {
        printf("command finished with ERROR\n");
        return 0;
    }
}

static PyObject* CardDispenser_reset(CardDispenser* self, PyObject* args)
{
    execute_command(self, 0x30, 0x30, NULL, 0);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_issue_middle(CardDispenser* self, PyObject* args)
{
    unsigned char data[] = {0x31};
    execute_command(self, 0x76, 0x44, data, 1);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_issue_middle_to_clamp(CardDispenser* self,
                                                     PyObject* args)
{
    unsigned char data[] = {0x31};
    execute_command(self, 0x76, 0x45, data, 1);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_issue_middle_to_fall(CardDispenser* self,
                                                    PyObject* args)
{
    unsigned char data[] = {0x31};
    execute_command(self, 0x76, 0x46, data, 1);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* CardDispenser_issue_clamp(CardDispenser* self, PyObject* args)
{
    unsigned char data[] = {0x31};
    execute_command(self, 0x76, 0x41, data, 1);
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
    {"issue_middle",
     (PyCFunction)CardDispenser_issue_middle, METH_NOARGS,
	 "dispense from card stack to the middle position"},
    {"issue_middle_to_clamp",
     (PyCFunction)CardDispenser_issue_middle_to_clamp, METH_NOARGS,
	 "dispense from middle position to clamp position"},
    {"issue_middle_to_fall",
     (PyCFunction)CardDispenser_issue_middle_to_fall, METH_NOARGS,
	 "dispense from middle position to falling out"},
    {"issue_clamp", (PyCFunction)CardDispenser_issue_clamp, METH_NOARGS,
	 "dispense to the clamp position"},
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

