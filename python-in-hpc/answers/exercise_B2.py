import numpy as np
import os
import matplotlib as mpl
mpl.use('Agg')
import pylab as pl
from mpi4py import MPI

comm = MPI.COMM_WORLD

def update(board):
    # number of neighbours that each square has
    neighbours = np.zeros(board.shape)
    neighbours[1:, 1:] += board[:-1, :-1]
    neighbours[1:, :-1] += board[:-1, 1:]
    neighbours[:-1, 1:] += board[1:, :-1]
    neighbours[:-1, :-1] += board[1:, 1:]
    neighbours[:-1, :] += board[1:, :]
    neighbours[1:, :] += board[:-1, :]
    neighbours[:, :-1] += board[:, 1:]
    neighbours[:, 1:] += board[:, :-1]

    new_board = np.where(neighbours < 2, 0, board)
    new_board = np.where(neighbours > 3, 0, new_board)
    new_board = np.where(neighbours == 3, 1, new_board)

    return new_board

# initialize global board
board = np.zeros((32, 32), int)
board[15,:] = 1
board[:,15] = 1

# board[:] = np.random.rand(32, 32).round(0)

# determine parallezation parameters
nprocs = comm.Get_size()
rank = comm.Get_rank()
loc_dim = 32 / nprocs
assert loc_dim * nprocs == 32
loc_board = np.zeros((loc_dim+2, 32), int) # need ghost layer

# Distribute initial board
comm.Scatter(board, loc_board[1:-1,:])

# find neighbouring processes
down = rank - 1
if down < 0:
    down = MPI.PROC_NULL
up = rank + 1
if up > nprocs-1:
    up = MPI.PROC_NULL

if rank == 0:
    pl.hold(False)
#    pl.imshow(board, cmap = pl.cm.prism)
for iter in range(30):
    # send up, receive from down
    sbuf = loc_board[-2,:]
    rbuf = loc_board[0,:]
    comm.Sendrecv(sbuf, dest=up, 
                 recvbuf=rbuf, source=down)
    # send down, receive from up
    sbuf = loc_board[1,:]
    rbuf = loc_board[-1,:]
    comm.Sendrecv(sbuf, dest=down, 
                 recvbuf=rbuf, source=up)

    # update the board
    loc_board = update(loc_board)

    # gather board to master and plot
    comm.Gather(loc_board[1:-1,:], board)
    if rank == 0:
        pl.imshow(board, cmap = pl.cm.prism)
        pl.savefig('game_{0:03d}.png'.format(iter))

# Create animated gif using Imagemagic
if rank == 0:
    os.system('convert game_???.png game.gif')
    os.system('rm -fr game_???.png')
