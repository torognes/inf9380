# Guidelines to build the chipseq workflow as a SLURM workflow
First, create the index files using ``bowtie-build`` then follow these guidelines:
1. Use ``pmap`` to do the bowtie alignemnt using MPI
-----------------------------------------------------
Lookup ``bowtie-with-mpi`` for details. Try to use a suitable number of MPI processes to get the best speedup with minimal parallelization overhead.
When you get the best number of processes ``N`` do the following:
* Get the overall time (both ``real`` and ``user``) for ``n = 1,...,N+2`` and draw a bar-chart.
* Run bowtie without MPI and make sure that the resulted ``sam`` file is identical to the one using ``pmap`` with ``mpirun -np N``

2. Create a SLURM script for each individual step
---------------------------------------------------
Create a SLURM script for each individual step, i.e. for processing one file, of the follwoing:
* Alignment
* Getting BAM file from SAM file
* Sorting the BAM file with samtools
* Getting bedGraph files with genomeCoverageBed
* Sorting bedGraph files based on column 1, then column 2
* Making bigWig files from bedGraph
* Peak calling
* Normalization, comparing N1 and T1 and making plots in R
Split/merge steps in SLURM scripts to achieve more speedup, e.g. things like ``cat`` and ``sort`` might not need to be submitted in separate SLURM jobs.

3. Create ``arrayrun`` scripts to run parallelizable steps in parallel
-----------------------------------------------------------------------
Parallelizable steps, e.g. alignment, are doing the same thing to multiple files at the same time. This is a typical case for ``arrayrun``. So, go ahead :)
