#!/bin/bash

#SBATCH --job-name=GPU_test
#SBATCH --account=ln0002K
#SBATCH --ntasks=1
#SBATCH --time=01:00
#SBATCH --mem-per-cpu=1G
## Ask for 1 GPU
#SBATCH --partition=accel --gres=gpu:1

## Set up job environment:
source /cluster/bin/jobsetup
set -o errexit # exit on errors

##Print number of available GPUs and some details about them
echo $CUDA_VISIBLE_DEVICES
nvidia-smi --query-gpu=name,memory.total,memory.free,memory.used --format=csv
