#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process  
  int world_rank;
  int master;  
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  master=(world_rank==0);

  if (master) {
    omp_proc_bind_t omp_get_proc_bind(void);
    printf("Environment:\n");
    switch (omp_get_proc_bind()) {    
    case 0 : printf("%s\n","proc binding false"); break;
    case 1 : printf("%s\n","proc binding true"); break;
    case 2 : printf("%s\n","proc binding master"); break;
    case 3 : printf("%s\n","proc binding close"); break;
    case 4 : printf("%s\n","proc binding spread"); break;
    default: printf("No binding\n");
    }    
  }
  
  // Print off a hello world message
  printf("Hello world from rank %d out of %d ranks\n",
	 world_rank, world_size);

    int threadno;
    int threads;  
#pragma omp parallel private(threadno) 
  {
    threadno=omp_get_thread_num();   // Get the thread ID
    threads=omp_get_num_threads();  // Get the total number of threads
    printf("  Hello world from thread %d out of %d threads on rank %d\n",
	   threadno, threads, world_rank );
  }
  // Finalize the MPI environment.
  MPI_Finalize();
}
