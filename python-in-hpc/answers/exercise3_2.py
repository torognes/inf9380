import numpy as np

arr = np.random.random((10,10))
print "Uniform distribution"
print "Mean: ", np.mean(arr), "Std. dev.: ", np.std(arr)

arr = np.random.exponential((10,10))
print "Exponential distribution"
print "Mean: ", np.mean(arr), "Std. dev.: ", np.std(arr)
