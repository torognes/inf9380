#!/bin/bash

#SBATCH --job-name=localtmp
#SBATCH --account=ln0002k
#SBATCH --time=00:12:00
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=1G

source /cluster/bin/jobsetup
set -o errexit

mkdir tmp
cp manyfiles.sh tmp/
cd tmp
./manyfiles.sh
cp all.txt ../
