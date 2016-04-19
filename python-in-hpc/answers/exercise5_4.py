from mpi4py import MPI
from pdb import PDB
from numpy import array, zeros

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

if rank == 0:
    pdb = PDB('5ire.pdb')
    n = len(pdb)
    # how many atoms should each process get?
    chunk = 3 * (n / size)
    left = 3 * (n % size)
    
    # calculate send counts and offsets
    counts = [chunk] * size
    counts[-1] += left
    counts = tuple(counts)
    offsets = [x * chunk for x in range(size)]
    offsets = tuple(offsets)

    # send chunk size to everyone and prepare a receive array
    comm.bcast(counts, root=0)
    comm.bcast(offsets, root=0)
    my_coords = zeros((counts[rank], 3), float)
    # scatter coordinates to all processes
    comm.Scatterv([pdb.coordinates, counts, offsets, MPI.DOUBLE], my_coords)

    # calculate the sum of coordinates
    result = sum(my_coords)

    # gather all partial results
    results = zeros((size, 3), float)
    comm.Gather(result, results)

    # calculate the global center of coordinates
    origo = sum(results) / len(pdb)

    # shift coordinates
    new_coordinates = pdb.coordinates - origo

    # broadcast the shifted coordinates to everyone
    comm.bcast(n, root=0)
    comm.Bcast(new_coordinates, root=0)

else:
    # receive chunk size and prepare a receive array
    counts = comm.bcast(None, root=0)
    offsets = comm.bcast(None, root=0)
    my_coords = zeros((counts[rank], 3), float)
    # scatter coordinates to all processes
    comm.Scatterv([None, counts, offsets, MPI.DOUBLE], my_coords)

    # calculate the sum of coordinates
    result = sum(my_coords)

    # gather all partial results
    comm.Gather(result, None)
    
    # receive the shifted coordinates from rank 0
    n = comm.bcast(None, root=0)
    new_coordinates = zeros((n, 3), float)
    comm.Bcast(new_coordinates, root=0)

print new_coordinates

