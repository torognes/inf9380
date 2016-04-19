from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

print "I'm rank {0} in group of {1} processes".format(rank, size)
