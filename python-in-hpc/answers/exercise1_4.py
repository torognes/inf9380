import numpy as np
import pylab

# read data
data = np.loadtxt('exercise9_4.dat')
pylab.imshow(data)

# split
data_upper, data_lower = np.split(data, 2)
data_upper_left, data_upper_right = np.split(data_upper, 2, axis=1)
data_lower_left, data_lower_right = np.split(data_lower, 2, axis=1)

pylab.figure()
pylab.imshow(data_upper_left)
pylab.figure()
pylab.imshow(data_upper_right)
pylab.figure()
pylab.imshow(data_lower_left)
pylab.figure()
pylab.imshow(data_lower_right)

# assemble again
data_left = np.concatenate((data_upper_left, data_lower_left))
data_right= np.concatenate((data_upper_right, data_lower_right))
data_new = np.concatenate((data_left, data_right), axis=1)

pylab.figure()
pylab.imshow(data_new)
pylab.show()

