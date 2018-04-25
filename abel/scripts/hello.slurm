#!/bin/bash

#SBATCH --job-name=hello
#SBATCH --account=ln0002k
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=256M
#SBATCH --time=00:01:00

source /cluster/bin/jobsetup
set -o errexit

sleep 1
echo “Hello from ” 
hostname 

