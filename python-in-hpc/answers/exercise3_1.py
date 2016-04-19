import numpy as np
import sys

filename = sys.argv[1]
xy = np.loadtxt(filename)
x = xy[:,0]
y = xy[:,1]
p = np.polyfit(x, y, 2)
print "Fit is: {0:f}x^2 + {1:f}x {2:+f}".format(p[0], p[1], p[2])
