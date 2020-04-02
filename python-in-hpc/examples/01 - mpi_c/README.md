# Getting started

Running MPI programs on Linux (using Ubuntu which is all I know) requires installing the tools to get started.

On Linux : 

```
sudo apt -y install libopenmpi-dev openmpi-bin
```

This should install what you need in order to get the first C program you have up and running.

Build it first

```
mpiCC -o mpi_c main.cpp
```

Now run it

```
mpirun -np 4 mpi_c
```

For more information on what's in this program (main.cpp) visit [this link](https://mpitutorial.com/tutorials/mpi-hello-world/)
