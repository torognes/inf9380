# Guidelines to build the chipseq workflow as a SLURM workflow
Please follow these guidelines:
1. Use ``pmap`` to do the bowtie alignemnt using MPI
-----------------------------------------------------
Lookup ``bowtie-with-mpi`` for details. Try to use a suitable number of MPI processes to get the best speedup with minimal parallelization overhead.
When you get the best number of processes ``N`` do the following:
* Get the overall time (both ``real`` and ``user``) for ``n = 1,...,N+2`` and draw a bar-chart.
* Run bowtie without MPI and make sure that the resulted ``sam`` file is identical to the one using ``pmap`` with ``mpirun -np N``
