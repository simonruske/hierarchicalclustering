#include <Python.h>
#include <numpy/arrayobject.h>

static PyObject* add(PyObject* self, PyObject* args) 
{
    PyObject *arg1 = NULL, *arg2 = NULL;
    
    // Parse input objects as arrays
    if (!PyArg_ParseTuple(args, "OO", &arg1, &arg2))
    {
        return NULL;
    }

    // Check if the input arguments are numpy arrays
    if (!PyArray_Check(arg1) || !PyArray_Check(arg2))
    {
        PyErr_SetString(PyExc_TypeError, "Input arguments must be numpy arrays.");
        return NULL;
    }

    // Convert to continguous arrays
    PyObject *arr1 = NULL, *arr2 = NULL;
    arr1 = PyArray_FROM_OTF(arg1, NPY_DOUBLE, NPY_IN_ARRAY);
    arr2 = PyArray_FROM_OTF(arg2, NPY_DOUBLE, NPY_IN_ARRAY);

    if (!arr1 || !arr2) 
    {
        Py_XDECREF(arr1);
        Py_XDECREF(arr2);
        return NULL;
    }

    npy_intp firstSize = PyArray_DIM(arr1, 0);
    npy_intp secondSize = PyArray_DIM(arr2, 0);

    if (firstSize != secondSize)
    {
        PyErr_SetString(PyExc_ValueError, "Arrays must be of the same size.");
        Py_DECREF(arr1);
        Py_DECREF(arr2);
        return NULL;
    }

    

    PyObject *result = PyArray_SimpleNew(1, &firstSize, NPY_DOUBLE);
    if (!result) {
        Py_DECREF(arr1);
        Py_DECREF(arr2);
        return NULL;
    }

    for (npy_intp i = 0; i < firstSize; i++)
    {
        double val1 = *(double*)PyArray_GETPTR1(arr1, i);
        double val2 = *(double*)PyArray_GETPTR1(arr2, i);
        *(double *)PyArray_GETPTR1(result, i) = val1 + val2;
    }

    Py_DECREF(arr1);
    Py_DECREF(arr2);

    return result;
}

static PyMethodDef ExampleMethods[] = 
{
    {"add", add, METH_VARARGS, "Add two numbers"},
    {NULL, NULL, 0, NULL } // Sentinel
};

static struct PyModuleDef examplemodule = 
{
    PyModuleDef_HEAD_INIT,
    "example", // name of module
    NULL, // module documentation, may be NULL
    -1, // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    ExampleMethods
};

PyMODINIT_FUNC PyInit_example(void) 
{
    PyObject *module;
    module = PyModule_Create(&examplemodule);
    if (!module) return NULL;
    import_array();
    return module;
}