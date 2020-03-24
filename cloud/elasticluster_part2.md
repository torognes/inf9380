Create your cluster and submit a test job
==========================================

You are finally ready to work on the cluster.


## 1. On the admin-server: Create an Elasticluster configuration file for the cluster

```
[cloud/nrec]
provider=openstack
auth_url=https://api.uh-iaas.no:5000/v3
username=#your-username
password=#your-pwd
project_name=uio-itf-inf9380
user_domain_name=dataporten
project_domain_name=dataporten
region_name=osl
identity_api_version=3

[login/centos]
image_user=centos
image_user_sudo=root
image_sudo=True
user_key_name=inf9380-ssh
user_key_private=~/.ssh/inf9380-ssh
user_key_public=~/.ssh/inf9380-ssh.pub

[setup/slurm]
slow_but_safer=True
provider=ansible
global_var_multiuser_cluster=no
login_groups=slurm_master
compute_groups=slurm_worker,julia

#edit clustername according to your username
[cluster/studentNN]
cloud=nrec
login=centos
setup=ansible
security_group=inf9380
login_nodes=1
compute_nodes=2
ssh_to=login
network_ids=62421b56-346d-4794-99b0-fc27fe4e700f
image_id=0f6d4a45-043b-4231-872d-4c8f1aee34fc

#edit clustername according to your username
[cluster/studentNN/login]
flavor=m1.small

#edit clustername according to your username
[cluster/studentNN/compute]
flavor=m1.small

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
