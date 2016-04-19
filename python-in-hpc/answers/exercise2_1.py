import numpy as np

# Construct xi
dx = 0.10
xi = np.arange(0, np.pi/2, dx)

# Function values
fi = np.sin(xi)
# Derivative
dfi = (fi[2:] - fi[:-2]) / (2.0*dx)

# Compare to cos. Note that derivative was not defined in end points
f_ref = np.cos(xi[1:-1])
print "D sin:"
print dfi
print "cos:"
print f_ref
print "Mean squared difference:"
print np.sqrt(np.sum((dfi - f_ref)**2))

