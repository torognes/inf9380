#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"


//#define N    3000000000 
#define N    2000000000 
//#define N 1800000000  

int main(int argc, char* argv[]) {
  int rank,master,size,err;
  double t;
  long  i,l;
  double *p;
  MPI_Status  st;
  MPI_Aint k;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  master=(rank==0);

  l=N;
  k=N;
  printf("Myrank is %d of %d\n",rank,size);
  if ((size!=2) && (master)) {
    printf("Only 2 ranks, -np 2 will work, exiting\n");
    MPI_Abort(MPI_COMM_WORLD, err);
  }
  if (master) printf("size MPI_Aint %ld\n",sizeof(k));
  if (master) printf("size l=%ld\n",sizeof(l));

  if (master) printf("l= %ld  log2(l)=%lf\n",l,log2(l)); 

  p = malloc(sizeof(double)*l); /* Need the buffer on both ranks */
  
  if (master) printf("Size of p %d MiB\n",(int)((sizeof(double)*l)/(1024*1024)));
  if (master) for (i=0;i < l; i++) p[i]=(double)1.0*i; /* Fill only rank 0 */
  t = MPI_Wtime(); /* Initiate the timer */
  
  /* We try to send array p with k elements, k & l are equal to N 
     What happen when k is too large ? */
  
  if(master) MPI_Send(p, k, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
  else MPI_Recv(p, k, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &st);	       

  /* Print some statistics */
  if (master) {
    t=MPI_Wtime()-t; /* secure the time */
    printf("It took %lf seconds to transfer the data\n",t);
    printf("Yielding a bandwidth of %8.2lf MiB/s\n",((sizeof(double)*l)/(1024*1024))/t);
  }
  /* is p[] identical on both rank 0 & 1 */
  printf("p[N-1] rank %d %lf\n",rank,p[N-1]); 
  free(p); /* Be nice to the OS */
    
  MPI_Finalize();
}

