from mayavi import mlab
from numpy import pi, sin, cos, mgrid

delta = pi/250.0
phi, theta = mgrid[0:pi:delta, 0:2*pi:delta]

r = sin(4*phi)**3 + cos(2*phi)**3 + sin(6*theta)**2 + cos(6*theta)**4
x = r*sin(phi)*cos(theta)
y = r*cos(phi)
z = r*sin(phi)*sin(theta)

mlab.mesh(x,y,z)
mlab.show()
