#!/bin/bash

#SBATCH --job-name=RCS1115_hello
#SBATCH --account=staff
#SBATCH --time=00:02:00
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=256M

source /cluster/bin/jobsetup
set -o errexit

module purge 
module load Python/3.5.2-foss-2016b

arrayrun 1-3 array_worker.slurm
