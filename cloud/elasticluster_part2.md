Create your cluster and submit a test job
==========================================

You are finally ready to work on the cluster.


## 1. On the admin-server: Create an Elasticluster configuration file for the cluster

Download a semi-ready elasticluster config file and edit the parts indicated in the file: 

```
cd $HOME
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/elasticluster.config -O ~/.elasticluster/config
```

Then go into the configuration file and edit it, with nano, vi(m) or emacs. The parts that need editing are marked. Basically this is the username and password to NREC, and the name you will call your cluster. 


I will show here with emacs how to open the file: 
```
emacs -nw ~/.elasticluster/config
```


## 2. Activate the elasticluster virtualenv and set up authentication
In [part 1, step 5](https://github.com/torognes/inf9380/blob/master/cloud/elasticluster_part1.md#5-prepare-access-to-nrec-openstack-for-elasticluster-to-work) we downloaded the keystone.sh file and edited the username and password part of the file. 

Now make sure to source it so that elasticluster can see the environment variables that this file sets up. 

```
cd $HOME
. elasticluster/bin/activate
source keystone.sh
```

## 3. Run elasticluster to create the cluster
``` 
elasticluster start studentNN
```
where studentNN is your cluster name

So if you are student01 you should do
```
elasticluster start student01
```
Make sure that you have edited your ```.elasticluster/config```file in step 1 properly, exchanging the student00 with your student number. 

This takes a while - ca 10min

## 4. Login to the cluster studentNN
From the admin node: 
```
elasticluster ssh <studentNN>
``` 

From outside:
We have set up this cluster with IPV6 public ip adress only (due to lack of public IPv6 adresses). 
Therefore, you can only log into the frontend machine from outside NREC if your network is IPv6 enabled.

Get the IPv6 adress of the frontend machine. For instance get it from elasticluster on the admin node
```
elasticluster list-nodes <clustername>
```
where you exchange the <clustername> with the name of your actual cluster. In your case e.g. student01
  
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
