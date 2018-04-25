#!/bin/bash

#SBATCH --job-name=mpi_test
#SBATCH --account=ln0002k
#SBATCH --ntasks=4
#SBATCH --mem-per-cpu=256M
#SBATCH --time=00:01:00

source /cluster/bin/jobsetup
set -o errexit

module purge 
module load Python/3.5.2-foss-2016b

mpirun python mpi_test.py
