from mpi4py import MPI
from fasta import Fasta

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

if rank == 0:
    fasta = Fasta('5ire.fasta.txt')
    chain = fasta['C']
    comm.send(chain, dest=1, tag=66)
elif rank == 1:
    chain = comm.recv(source=0, tag=66)
    print 'Received:'
    print chain

