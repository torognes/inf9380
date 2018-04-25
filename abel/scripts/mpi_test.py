#Python script to show how to submit a MPI job

from mpi4py import MPI 

comm = MPI.COMM_WORLD # communicator object containing all processes 
size = comm.Get_size() 
rank = comm.Get_rank() 
print("I am rank %d in group of %d processes" % (rank, size))
