/* MPI program that uses a monte carlo method to compute the value of PI */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#define USE_MPI   

main(int argc, char *argv[])
{
   int niter=0;
   double x,y;
   int i,j,count=0,mycount; /* # of points in the 1st quadrant of unit circle */
   double z;
   double pi;
   int myid,numprocs,proc;
   MPI_Status status;
   int master =0;
   int tag = 123;
   
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);
   
   if (argc <=1) {
     fprintf(stderr,"Usage: pi.x number_of_iterations\n");
     MPI_Finalize();
     exit(-1);
   }
    
   sscanf(argv[1],"%d",&niter); /* 1st argument is the number of iterations*/

   srand(myid);
   mycount=0;
   for ( i=0; i<niter; i++) {     
     x=rand()/(RAND_MAX+1.0);
     y=rand()/(RAND_MAX+1.0);
      z = x*x+y*y;
      if (z<=1) mycount++; 
   }
   if (myid ==0) { /* if I am the master process gather results from others */
      count = mycount;
      for (proc=1; proc<numprocs; proc++) {
         MPI_Recv(&mycount,1,MPI_REAL,proc,tag,MPI_COMM_WORLD,&status);
         count +=mycount;        
      }
      pi=(double)count/(niter*numprocs)*4;
      printf("\n # of trials= %d , estimate of pi is %g \n",niter*numprocs,pi);
   }
   else { /* for all the slave processes send results to the master */
      printf("Processor %d sending results= %d to master process\n",myid,mycount
      );
      MPI_Send(&mycount,1,MPI_REAL,master,tag,MPI_COMM_WORLD);
   }
   
   MPI_Finalize();             /* let MPI finish up */
  
}
