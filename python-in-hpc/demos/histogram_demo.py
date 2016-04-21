#!/usr/bin/env python
import numpy as np
import matplotlib.mlab as mlab
import pylab

mu, sigma = 100, 15
x = mu + sigma*np.random.randn(10000)

# the histogram of the data
n, bins, patches = pylab.hist(x, 50, normed=1, facecolor='green', alpha=0.75)

# add a 'best fit' line
y = mlab.normpdf( bins, mu, sigma)
l = pylab.plot(bins, y, 'r--', linewidth=1)

pylab.xlabel('Smarts')
pylab.ylabel('Probability')
pylab.title(r'$\mathrm{Histogram\ of\ IQ:}\ \mu=100,\ \sigma=15$')
pylab.axis([40, 160, 0, 0.03])
pylab.grid(True)

pylab.show()
