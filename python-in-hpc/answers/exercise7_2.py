from timeit import Timer

setup = '''
import numpy as np
from fidi import fidi as cfidi
from pyfidi import fidi as pyfidi

a = 0
b = 1000.0
h = 0.02
x = np.arange(a, b+h, h)
rho = 2 * x**2 - 1
phi = np.zeros_like(rho)
'''

t0 = Timer('pyfidi(rho, h, phi)', setup).timeit(10000)
t1 = Timer('cfidi(rho, h, phi)', setup).timeit(10000)
print
print "Python:", t0
print "     C:", t1

