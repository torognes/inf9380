#!/bin/bash

#SBATCH --job-name=localtmp
#SBATCH --account=ln0002k
#SBATCH --time=00:12:00
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=1G
#SBATCH --gres=localtmp:1

source /cluster/bin/jobsetup
set -o errexit

echo $SUBMITDIR
echo $LOCALTMP
df -h $LOCALTMP

cp manyfiles.sh $LOCALTMP
cd $LOCALTMP
pwd
./manyfiles.sh
rsync all.txt $SUBMITDIR/

