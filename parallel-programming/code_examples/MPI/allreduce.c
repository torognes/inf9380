#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
  int        rank, size, err;
  double     *sendv, *recv;
  int        i,j,n;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
   
  n=10;
  sendv=malloc(n*sizeof(double));
  recv=malloc(n*sizeof(double));

  for(i=0;i<n;i++) sendv[i]=rank+1;

  MPI_Allreduce(sendv, recv, n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  if (rank==0) for(i=0;i<n;i++) printf("sendv(%d) recv(%d) %lf %lf\n",i,i,sendv[i],recv[i]);


  MPI_Finalize();
}

  
