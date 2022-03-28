# Overview

In this hands-on tutorial we will be using Terraform to create the resources in NREC and ansible to configure the resources. 

The steps to achieve this will be:

1. Set up
2. Install some general useful software (text-editors, git etc)
3. Install Terraform
4. Install Ansible and Openstack client
5. Set up authentication to communicate with NREC
6. Create resources with Terraform
7. Configure resources with Ansible
8. Run a hello-world example with openmpi

# 1. Set up: Prepare your admin machine 

The admin machine will function as the machine you use to create and configure the machines in NREC. 
You could potentially instead use your laptop, but that would require lots of different setups, fitting each laptop (Windows, Mac, Linux, different versions etc). 

We will be working on CentOS Stream 8 servers (could be anything else, but that is my preferred OS). 

## Fetch the ssh keys you will be using later on. Log into the admin machine

(You got the keypair sent to you by email.)

1. Save the key-pairs you got in the email from Torbjørn Thursday 24.03 onto your laptop's Desktop (or any other place you prefer. If you are on a linux or Mac you can copy them directly into your `~/.ssh` folder)

3. From your laptop copy the keys to the admin machine on NREC. 

   ```scp -i <folder-you-stored-the-key-pairs>/inf9380-2022-ssh <folder-you-stored-the-key-pairs>/inf9380-2022-ssh* centos@<your-admin-ip>:~/.ssh/``` 
   
   Example:
   
   ```scp -i ~/Desktop/inf9380-2022-ssh ~/Desktop/inf9380-2022-ssh* centos@158.39.75.164:~/.ssh/``` 
   
3. Log into the admin machine 
   
   ```ssh -i <folder-you-stored-the-key-pairs>/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 
   
   Example: 
   
   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@158.39.201.196```
   
   
4. On the admin machine - change the permissions of the key file (protect it)

   ```chmod 0600 ~/.ssh/inf9380-2022-ssh```

The private key will be used to connect from the admin machine to the new instances you will be creating for the cluster. 

## Fetch the github repository that contains templates and scripts you will be using 
You find your machine here: https://github.com/torognes/inf9380/blob/master/cloud/admin_machines.md

1. Log into the admin machine (if you have not already) 
   
   ```ssh -i <folder-you-stored-the-key-pairs>/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 
   
   Example:
   
   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@158.39.201.196```

2. Install git

   ``` sudo dnf install -y git```

3. Clone the repo we will be using in this hands-on session

   ```git clone https://github.uio.no/maikenp/inf9380_cloudscripts.git```




## 2. Install some general useful software
On the admin machine do:

```sudo dnf install -y wget unzip emacs nano python3```


# Install software for working with openstack 

## 3. Install Terraform. 
Terraform is an open-source infrastructure as code software tool that provides a consistent CLI workflow to manage hundreds of cloud services. 

On the admin machine, install terraform:

```
sudo dnf install -y dnf-utils
sudo dnf config-manager --add-repo https://rpm.releases.hashicorp.com/RHEL/hashicorp.repo
sudo dnf install -y terraform 
```

## 4. Install Ansible and the Openstack client 

Openstack client for useful cli (command-line-interface) to openstack. 

Ansible is an open-source software provisioning, configuration management, and application-deployment tool enabling infrastructure as code. 
We will use it to configure our NREC instances. 

On the admin machine do:

``` 
sudo dnf install -y centos-release-openstack-ussuri
sudo dnf install -y ansible python3-openstackclient
``` 

(openstack-ussuri provides ansible and openstack client) 


## 5. Set up authentication to communicate with NREC

Test to see if you can communicate with NREC:

```openstack server list | grep student02``` 

Does not work, you are missing authentication. 


1. Fill in the missing info in *keystonerc* file (username and password)
   
   First copy the file to your homefolder, so it is not overwritten if you later do some git updates. 
   
   ```cp $HOME/inf9380_cloudscripts/create/keystonerc ~/```
   
   Your NREC username and password has been sent to you in email from Torbjørn last week. The username and password must be filled out in the empty field of the keystonerc file you just copied to your home folder. 
      
      ```cd ~/```
      
    Add the username to the OS_USERNAME variable, and the password to the OS_PASSWORD variable. 

2. Source the file to create environment variables with the connection and authentication information in the keystone file

    ```source ~/keystonerc``` 
    
3. Now environment variables are set with the authentication information (username and password) for terraform to communicate with NREC openstack. 

   ```printenv | grep OS_``` 

4. Now try the openstack command again

   ```openstack server list | grep student```    
    
    
**If you log out of the admin machine, and log in again, you will have to source the keystonerc file again. The variables we set up are flushed from the environment once we log out.**

# Use terraform and ansible to respectively create and configure the instances

Now we have installed necessary software and have set up our authentication (by filling in the keystonerc file and sourcing it). 

Next step is to actually use the software to create our cluster. 
We will now be working only from the NREC admin machine. 


## 6. Create resources with Terraform
 
1. If you are not already in your admin machine, do:

   ```ssh -i <folder-you-stored-the-key-pairs>/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 

2. If you have not already done so, source the keystone file
   ```source ~/keystonerc``` 

3. Create a folder on your desktop where you will be working with terraform. Copy the basic.tf file from the create folder:
   ```
   cd
   mkdir terraform
   cd terraform
   cp $HOME/inf9380_cloudscripts/basic.tf .
   ``` 

4. Open the basic.tf file with either nano, vi or emacs (chose the text-editor you prefer of these 3 available)

   ```emacs -nw basic.tf``` 

5. Replace "studentXX" with your number in the "name" field in the resource block.

6. Notice the security group block. The default is always added. In addition we have the inf9380 security group which I have created in advance in NREC. It opens up all ports between the machines in the cluster, so that the machines can communicate among each other, in addition to allowing ssh access to the instances. The rules look like this: 
 
![Screenshot 2022-03-28 at 10 59 40](https://user-images.githubusercontent.com/22190352/160363634-a6ea65e8-dd27-45fe-9cd4-21dbeff327da.png)


7. For openstack cli to work we also need to create the ssh keypair via the openstack cli. Create the key. The syntax is `openstack keypair create --pubcli-key <path-to-your-public-key> <name-you want to give it>` 
    
   ```openstack keypair create --public-key ~/.ssh/inf9380-2022-ssh.pub inf9380-2022-ssh```
    
8. Keep the rest as it is in basic.tf - although you can change things if you want, for instance create larger compute nodes, e.g change the flavor. Explore the openstack command to see what options there are `openstack -h`. For example
   `openstack flavor list` will give you the list of flavors available to you and you will see the amount of RAM, Disk anc VCPUs that each flavor has. 

9. Prepare your workin directory 

   ```terraform init```
 
10. Show what changes will be performed once you run apply

   ```terraform plan```

11. If everything seems ok (verify that you got correct names of the instances for example), run:
    
    ```terraform apply -auto-approve```

12. Check that it worked - replace XX with your student number
 
    ```openstack server list | grep studentXX```
    
    You should see a list of 3 instances. 

The instances are now created in openstack and you can ssh into them if you want


## 7. Configure resources with Ansible
The resources are created, but they dont have any software installed yet. Also, we are going to be running an openmpi application - and to avoid having to copy files from the admin machine to the compute nodes, we want to set up a shared filesystem. 

For ease of work, let's put the ssh-key into the ssh-agent. Then we can skip the ```--private-key``` option in the ansible commands. 
**Note: you will have to do this everytime at login** 

### a) 
```
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/inf9380-2022-ssh  
``` 

### b) 
Create a workdir where you will be running the ansible commands. Again let's copy the necessary files from the git folder, so that we do no accidentally overwrite them if we update the folder via git pull at some point. 

``` 
mkdir ~/ansible
cd ~/ansible
cp ~/inf9380_cloudscripts/configure/* .
```

 
Inspect install_openmpi.yml and openmpi.sh.  You see that this is a playbook with the tasks directly written in the playbook. It installs some prerequisites for openmpi and openmpi itself. We also have to make sure that the system knows where to find openmpi, and we therefore have to add the location to the path. 

### c) 
You need to edit the inventory file "ansible_hosts" that we copied from the configure folder. It now contains some example host names and ip's. You need to change those according to your machines. 

You can get a list of the names and ip-s doing 

```openstack server list -c Networks -c Name | grep student<yournumber>```
where you change `<yournumber>` with your actual number, e.g. student40. 

copy paste the names and corresponding ip's to replace the ones in the ansible_hosts file. 

### d)
Now run:

 ``` 
ANSIBLE_HOST_KEY_CHECKING=False  ansible-playbook -i ansible_hosts install_openmpi.yml 
```
 
 

### e) Set up a shared filesystem

To be able to work smoothly, and not having to copy files from the master machine to the compute nodes, lets set up a shared filesystem. 

Inspect setup_nfs.yml and exportfs.j2. 

Now run:

 ``` 
ANSIBLE_HOST_KEY_CHECKING=False  ansible-playbook -i ansible_hosts setup_nfs.yml 
```

You are done setting up your instances. 
 
## 9. Run a hello-world example with openmpi
 
Now we are ready to try mpirun running over all the 3 instances. 

All the 3 instances must have openmpi installed, and the hello world executable, ssh-authentcation to the nodes must aslo be set up. 

We already installed openmpi on all the machines in step 7. 
We also set up a shared file system. The ```/scratch``` folder is exported from the admin machine to the two other instances. We will use that instead of copying the hello world executable to each machine with `scp`. 
  
  
### a) 
If you have not already done so, run:

```
 eval "$(ssh-agent -s)"
 ssh-add ~/.ssh/inf9380-2022-ssh  
```

### b) 
Copy the code you will use to the shared folder  `/scratch`

On the admin machine:
```
cp $HOME/inf9380_cloudscripts/application/hello.f90 /scratch/
cp $HOME/inf9380_cloudscripts/application/mpi_hello_world.c /scratch/
cd /scratch/
```

We can log into one of the compute nodes to verify that the /scratch folder with its contents is indeed shared: 

```
ssh <ips-of-one-of-your-machines>
df -h
ls /scratch/
exit
```

Yes, the file should be there and we are all set. 


### c) 
Now make a host-file so that mpi knows what machines to run on. 

```
cd /scratch
touch hosts.txt
```

Get a list of your servers:

```openstack server list | grep student<your-number>```

Copy the ipv4 addresses of your instances into the text file hosts.txt you just created in scratch /scratch using your favourite text editor. 
And example of the contents:

```
158.39.48.1
158.39.200.2
158.37.63.3
```
 
### Fortran hello-world execution
Compile and run the Fortran hello-world script. 
```
mpif90 -o hello.x hello.f90
mpirun -np 3  -hostfile hosts.txt ./hello.x
``` 


You should see output of the type: 

```
admin-student38.novalocal      I am master, and my rank is:   0
admin-student38.novalocal     Myrank is number    0 of total ranks   3
student38-compute0.novalocal  Myrank is number    1 of total ranks   3
student38-compute1.novalocal  Myrank is number    2 of total ranks   3
``` 


### C hello-world execution

Compile and run the c hello-world script:

```
mpic++ -o mpi_hello_world.x mpi_hello_world.c 
mpirun -n 3 -hostfile hosts.txt ./mpi_hello_world.x
```
You should see output of the type: 

```
Hello world from processor admin-student38.novalocal, rank 0 out of 3 processors
Hello world from processor student38-compute1.novalocal, rank 2 out of 3 processors
Hello world from processor student38-compute0.novalocal, rank 1 out of 3 processors
```

### Troubleshooting:

If you see a message starting with :
```
[centos@admin-student38 scratch]$ mpirun -np 3  -hostfile hosts.txt hello.x
centos@158.39.200.195: Permission denied (publickey,gssapi-keyex,gssapi-with-mic).
```

That indicates that you forget to set up your ssh key. 

Add your key to the ssh-agent, and that should fix that problem

```
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/inf9380-2022-ssh
```




# All done
You have successfully created some resources (instances) in openstack with Terraform, you have configured the instances by installing some software and setting up shared filesystem with ansible, and you have run an openmpi application over the 3 machines in paralell. 

