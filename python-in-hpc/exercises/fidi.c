/*
 *  Copyright (C) 2010-2012     CSC - IT Center for Science Ltd.
 *  Please see the accompanying LICENSE file for further information. */

/* Simple C interface for calculating second derivative with
   finite differences */

#include <Python.h>
#define PY_ARRAY_UNIQUE_SYMBOL GPAW_ARRAY_AP
#define NO_IMPORT_ARRAY
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

  n = data_in->dimensions[0];
  in = (double*) PyArray_DATA(data_in);
  out = (double*) PyArray_DATA(data_out);
  h2inv = 1.0 / (h*h);
  out[0] = (-2.0 * in[0] + in[1]) * h2inv;
  for (i=1; i < n-1; i++)
    {
      out[i] = (in[i-1] - 2.0 * in[i] + in[i+1]) * h2inv;
    }      
  out[n-1] = (in[n-2] -2.0 * in[n-1]) * h2inv;

  Py_RETURN_NONE;
}

/* Create here PyMethodDef and define the Python interface for 
 * fidi function */
static PyMethodDef functions[] = {
  /* Fill in */
};

/* Create the module initialization function initfidi */

  
