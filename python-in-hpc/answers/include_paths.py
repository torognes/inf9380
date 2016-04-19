from distutils.sysconfig import get_python_inc
import numpy

print "-I%s -I%s" % (get_python_inc(), numpy.get_include())
