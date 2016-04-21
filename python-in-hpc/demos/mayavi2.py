from mayavi import mlab
import numpy as np

x, y, z = np.ogrid[-5:5:64j, -5:5:64j, -5:5:64j]
scalars = 0.5*x**2 + y**2 + 2.0*z**2

mlab.contour3d(scalars, contours=4, transparent=True)
mlab.show()

