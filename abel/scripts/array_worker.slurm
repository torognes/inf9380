#!/bin/bash

#SBATCH --job-name=Arrayrun_hello
#SBATCH --account=ln0002k
#SBATCH --time=00:01:05
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=256M

source /cluster/bin/jobsetup
set -o errexit

EXTENTION=".txt"
RESULT=$TASK_ID$EXTENTION

python --version
python array_test.py input.txt $RESULT $TASK_ID

