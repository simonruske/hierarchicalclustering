
#include <vector>
#include <Python.h>
#include "Queue.h"

typedef struct {
    PyObject_HEAD
    Queue* obj;
} PyQueue;

static int PyQueue_init(PyQueue* self, PyObject* args, PyObject* kwds) {
    self->obj = new Queue(); // Allocate the C++ object
    return 0; // Success
}

static void PyQueue_dealloc(PyQueue* self) {
    delete self->obj;
    Py_TYPE(self)->tp_free((PyObject*)self);
};

static PyObject* PyQueue_AddToArray1(
    PyQueue* self,
    PyObject* args
) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    self->obj->AddToArray1(value);
    Py_RETURN_NONE;
}

static PyObject* PyQueue_GetObjectAtIndex(
    PyQueue* self,
    PyObject* args
) {
    int index;
    if (!PyArg_ParseTuple(args, "i", &index)) {
        return NULL;
    }
   
    int value = self->obj->GetValueAtLocation(index);

    return PyLong_FromLong((long)value);   

}

static PyObject* PyMyCppObj_AddToArray2(
    PyQueue* self,
    PyObject* args
) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    self->obj->AddToArray2(value);
    Py_RETURN_NONE;
}

static PyMethodDef PyQueue_methods[] = {
    {"add_to_array1", (PyCFunction)PyQueue_AddToArray1, METH_VARARGS, "Add a value to array 1"},
    {"get_value_in_array1", (PyCFunction)PyQueue_GetObjectAtIndex, METH_VARARGS, "Get a value from array 1"},
    {NULL} /* Sentinel */
};

static PyTypeObject PyQueueType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mymodule.Queue",               /* tp_name */
    sizeof(PyQueue),                /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyQueue_dealloc,    /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    "Queue objects",                /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyQueue_methods,                /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyQueue_init,         /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

static PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    "mymodule",
    "Example module that includes a C++ object with two arrays",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_mymodules(void) {
    PyObject* m;
    if (PyType_Ready(&PyQueueType) < 0) {
        return NULL;
    }

    m = PyModule_Create(&mymodule);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&PyQueueType);
    if (PyModule_AddObject(m, "PyQueue", (PyObject*)&PyQueueType) < 0) {
        Py_DECREF(&PyQueueType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}