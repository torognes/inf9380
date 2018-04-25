#!/bin/bash

#SBATCH --job-name=RCS1115_hello
#SBATCH --account=staff
#SBATCH --time=00:01:00
#SBATCH --ntasks=3
#SBATCH --mem-per-cpu=256M

FILES=(/path/to/data/*)

source /cluster/bin/jobsetup
set -o errexit

echo "number of instances "$SLURM_NTASKS
srun aprogram.sh


