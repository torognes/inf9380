# Guidelines to build the chipseq workflow as a SLURM workflow (For everybody)

Use the data in ``/work/projects/norbis/exam/reference_data`` and ``/work/projects/norbis/exam/data``

First, create the index files using ``bowtie-build`` then follow these guidelines:

1. Use ``pmap`` to do the bowtie alignemnt using MPI
-----------------------------------------------------
Lookup [``bowtie-with-mpi``](bowtie-with-mpi) for details. Try to use a suitable number of MPI processes to get the best speedup with minimal parallelization overhead.
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

Split/merge steps in SLURM scripts to achieve more speedup, e.g. things like ``cat`` and ``sort`` might not need to be submitted in separate SLURM jobs. For the R step, create an R-script and include copying it to ``$SCRATCH`` in your SLURM script, and don't forget to load the R module.

3. Create ``arrayrun`` scripts to run parallelizable steps in parallel
-----------------------------------------------------------------------
Parallelizable steps, e.g. alignment, are doing the same thing to multiple files at the same time. This is a typical case for ``arrayrun``. So, go ahead :)

Report the overall time (both ``real`` and ``user``) for running each of those steps with and without using ``arrayrun``

4. Create a ``sdag`` script for the whole workflow
---------------------------------------------------
You are almost there :)

* Create a ``sdag`` script for the whole workflow. Define a ``JOB XX`` for each step. An ``arrayrun`` script should also be defined as a single ``sdag`` job.
* Submit your ``sdag`` workflow and report the overall time. Here calculating the time is a bit tricky since the ``sdag`` command returns immediately. So, do the following: 
  * Get the job starting time of the first job of the workflow, e.g. ``Starting job ... on ... at <TIME>``
  * Subtract it from the finishing time of the last job in the workflow, e.g. ``Job ... completed on ... at <TIME>``
  * Here you can report only one overall time (no ``real`` or ``user``)
  * The start/finishing times can be found on the file ``slurm-<job-id>.out``, and as you know ``sdag`` gives you the IDs of all submitted workflow jobs.
* Repeat the previous with a ``sdag`` workflow without using ``arrayrun`` but with bowtie and MPI, and another time without ``arrayrun`` and without MPI and report the time.

5. Create a setup manual
-------------------------
Now you are done :)

Create a manual on how to use your scripts to reproduce your results.
