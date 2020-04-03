FOPT  = -O0 -fopenmp 
COPT  = -O2
FC	= gfortran
CC      = gcc 
THREADS = ${OMP_NUM_THREADS}
all:            false-sharing.x 

mysecond.o:
	$(CC) $(COPT) -c -DUNDERSCORE mysecond.c

false-sharing.x: false-sharing.F90 mysecond.o
	$(FC) $(FOPT) -DTHREADS=$(THREADS) -o false-sharing.x false-sharing.F90 mysecond.o

run:	Makefile false-sharing.x 
	OMP_NUM_THREADS=$(THREADS) /usr/bin/time ./false-sharing.x 

clean:
	rm -f *.o false-sharing.x
