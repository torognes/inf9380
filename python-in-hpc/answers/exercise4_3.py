import pylab as pl
import numpy as np

data = np.loadtxt('contour.dat')
fig = pl.figure()
pl.contourf(data)
pl.axis('tight')
pl.show()

