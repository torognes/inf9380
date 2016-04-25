import numpy as np
from matplotlib import pylab as pl

x = np.linspace(-np.pi/2, np.pi/2, 20)
y1 = np.sin(x)
y2 = np.cos(x)
pl.plot(x, y1, label='sin')
pl.plot(x, y2, label='cos')
pl.xlabel(r'$\theta$')
pl.legend(loc='upper left')
pl.savefig('sincos.png')
pl.show()
