import numpy as np

def fidi(f, h, phi):
    h2inv = 1./ h**2
    # Finite difference approximation for 2nd derivative in 1D
    phi[1:-1] = (f[:-2] - 2*f[1:-1] + f[2:]) * h2inv
    # Dirichlet boundary conditions
    phi[0] = (- 2*f[0] + f[1]) * h2inv
    phi[-1] = (f[-2] - 2*f[-1]) * h2inv

a = 0
b = 1000.0
h = 0.02
x = np.arange(a, b+h, h)
rho = 2 * x**2 - 1

phi = np.zeros_like(rho)
fidi(rho, h, phi)

print phi[1:-1]

