#!/bin/bash

#SBATCH --job-name=hello
#SBATCH --account=ln0002k
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:01:00

source /cluster/bin/jobsetup
set -o errexit

echo $SUBMITDIR
echo $SCRATCH
#Copy input files to the scratch directory
cp $SUBMITDIR/inputs/*txt $SCRATCH

#issue chkfile command to copy back results
#that will be created
chkfile output.txt

#process input files
cat *txt > output.txt

