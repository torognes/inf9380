#include <Python.h>

PyObject* passed_c(PyObject *self, PyObject *args)
{
  int a;
  double b;
  char* str;
  if (!PyArg_ParseTuple(args, "ids", &a, &b, &str))
    return NULL;
  printf("int %i, double %f, string %s\n", a, b, str);
  return Py_BuildValue("(ids)", a, b, str);
}

PyObject* square_c(PyObject *self, PyObject *args)
{
  int a;
  if (!PyArg_ParseTuple(args, "i", &a))
    return NULL;
  a = a*a;
  return Py_BuildValue("i", a);
}

PyObject* world_c(PyObject *self, PyObject *args)
{
  printf("Hello world!\n");
  Py_RETURN_NONE;
}

static PyMethodDef functions[] = {
 {"world", world_c, METH_VARARGS, 0},
 {"passed", passed_c, METH_VARARGS, 0},
 {"square", square_c, METH_VARARGS, 0},
 {0, 0, 0, 0}
};

PyMODINIT_FUNC inithello(void)
{
    (void) Py_InitModule("hello", functions);
}
