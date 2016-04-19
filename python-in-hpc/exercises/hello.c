#include <Python.h>

PyObject* world_c(PyObject *self, PyObject *args)
{
  printf("Hello world!\n");
  Py_RETURN_NONE;
}

static PyMethodDef functions[] = {
 {"world", world_c, METH_VARARGS, 0},
 {0, 0, 0, 0}
};

PyMODINIT_FUNC inithello(void)
{
    (void) Py_InitModule("hello", functions);
}
