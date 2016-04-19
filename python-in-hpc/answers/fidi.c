/*
 *  Copyright (C) 2010-2014     CSC - IT Center for Science Ltd.
 *  Please see the accompanying LICENSE file for further information. */

/*  Simple C interface for calculating second derivative with
    finite differences. Routine assumes zero boundary conditions for 
    the input data */

#include <Python.h>
#include <numpy/arrayobject.h>

PyObject* fidi(PyObject *self, PyObject *args)
{
  PyArrayObject* data_in;
  PyArrayObject* data_out;
  double h;
  int i, n;

  double* in;
  double* out;
  double h2inv;

  if (!PyArg_ParseTuple(args, "OdO", &data_in, &h, &data_out))
    return NULL;

  n = PyArray_SIZE(data_in);
  in = (double*) PyArray_DATA(data_in);
  out = (double*) PyArray_DATA(data_out);
  h2inv = 1.0 / (h*h);
  // Zero boundary conditions, i.e. in[-1] = 0.0
  out[0] = (-2.0 * in[0] + in[1]) * h2inv;
  for (i=1; i < n-1; i++)
    {
      out[i] = (in[i-1] - 2.0 * in[i] + in[i+1]) * h2inv;
    }      
  // Zero boundary conditions, i.e. in[n] = 0.0
  out[n-1] = (in[n-2] -2.0 * in[n-1]) * h2inv;

  Py_RETURN_NONE;
}

static PyMethodDef functions[] = {
  {"fidi", fidi, METH_VARARGS, 0},
  {0, 0, 0, 0}
};

PyMODINIT_FUNC initfidi(void)
{
  import_array();
  PyObject *m = Py_InitModule("fidi",functions);
}
