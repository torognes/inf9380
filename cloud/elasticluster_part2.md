Create your cluster and submit a test job
==========================================

You are finally ready to work on the cluster.


## 1. On the admin-server: Create an Elasticluster configuration file for the cluster

Download this file https://raw.githubusercontent.com/torognes/inf9380/master/cloud/elasticluster.config and edit the parts indicated in the file

```
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/elasticluster.config -O ~/.elasticluster/config
```


## 2. Activate the elasticluster virtualenv
```
cd $HOME
. elasticluster/bin/activate
```

## 3. Run elasticluster to create the cluster
elasticluster start studentNN
where studentNN is your cluster name

This takes a while - ca 10min

## 4. Login to the cluster studentNN
From the admin node: 
```
elasticluster ssh <studentNN>
``` 

From outside:
We have set up this cluster with IPV6 public ip adress only (due to lack of public IPv6 adresses). 
Therefore, you can only log into the frontend machine from outside NREC if your network is IPv6 enabled.

Get the IPv6 adress of the frontend machine
```
ssh -i ~/.ssh/inf9380-ssh centos@<ipv6-adress>
```


## 5. Check out your cluster with SLURM
```
sinfo
squeue
```


## 6. Submit a SLURM job

* Create the following script:

submit_job.sh

``` 
#!/bin/bash
#
#SBATCH --job-name=student00
#SBATCH --output=student00_slurmid_%j.out
#
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=100

srun hostname
srun hostname -i
srun sleep 60
```

* Run the script
```
sbatch  submit_job.sh
```

* Inspect the job
```
squeue
sinfo
scontrol show job <jobid>
```
