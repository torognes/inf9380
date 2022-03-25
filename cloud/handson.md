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

3. From your laptop copy the keys to the admin machine. 

   ```scp -i <folder-you-stored-the-key-pairs>/inf9380-2022-ssh <folder-you-stored-the-key-pairs>/inf9380-2022-ssh* centos@<your-admin-ip>:~/.ssh/``` 
   
   Example:
   
   ```scp -i ~/Desktop/inf9380-2022-ssh ~/Desktop/inf9380-2022-ssh* centos@158.39.75.164:~/.ssh/``` 
   
3. Log into the admin machine 
   
   ```ssh -i <folder-you-stored-the-key-paris>/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 
   
   Example: 
   
   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@158.39.201.196```
   
   
4. On the admin machine - change the permissions of the key file (protect it)

   ```chmod 0600 ~/.ssh/inf9380-2022-ssh```

The private key will be used to connect from the admin machine to the new instances you will be creating for the cluster. 

## Fetch the github repository that contains templates and scripts you will be using 
You find your machine here: https://github.com/torognes/inf9380/blob/master/cloud/admin_machines.md

1. Log into the admin machine
   
   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 
   
   Example:
   
   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@158.39.201.196```

2. Install git

   ``` sudo dnf install -y git```

3. Clone the repo

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

1. 
    ```cd $HOME/inf9380_cloudscripts/create``` 

2. Fill in the missing info in *keystonerc* file (username and password)
   
   First copy the file to your homefolder, so it is not overwritten if you later do some git updates. 
   
   ```cp $HOME/inf9380_cloudscripts/create/keystonerc ~/```
   
   Your NREC username and password has been sent to you in email from Torbjørn last week. The username and password must be filled out in the empty field of the keystonerc file you just copied to your home folder. 
      
      ```cd ~/```
      
    Add the username to the OS_USERNAME variable, and the password to the OS_PASSWORD variable. 

3. Source the file to create environment variables with the connection and authentication information in the keystone file

    ```source ~/keystonerc``` 
    
4. Now environment variables are set with the authentication information (username and password) for terraform to communicate with NREC openstack. 

   ```printenv | grep OS_``` 

5. Now try the openstack command again

   ```openstack server list | grep student```    
    

# Use terraform and ansible to respectively create and configure the instances

Now we have installed necessary software and have set up our authentication (by filling in the keystonerc file and sourcing it). 

Next step is to actually use the software to create our cluster. 
We will now be working only from the NREC admin machine. 


## 6. Create resources with Terraform
 
1. If you are not already in your admin machine, do:

   ```ssh -i ~/Desktop/inf9380-2022-ssh centos@<ip-of-your-admin-machine>``` 

2. Go to the correct folder: ```cd $HOME/inf9380_cloudscripts/create```

3. Open the basic.tf file with either nano, vi or emacs (chose the text-editor you prefer of these 3 available)

   ```emacs -nw basic.tf``` 

4. Replace "studentXX" with your number in the "name" field in the resource block.

5. Create the key: ```openstack keypair create --public-key ~/.ssh/inf9380-2022-ssh.pub inf9380-2022-ssh```
7. Keep the rest as it is

6. ```terraform init```
 
7. ```terraform plan```

8. ```terraform apply -auto-approve```

9. Check that it worked - replace XX with your student number
 
   ```openstack server list | grep studentXX```


The instances are now created in openstack and you can ssh into them if you want


## 7. Configure resources with Ansible
The resources are created, but they dont have any software installed yet. Also, we are going to be running an openmpi application - and to avoid having to copy files from the admin machine to the compute nodes, we want to set up a shared filesystem. 

For ease of work, let's put the ssh-key into the ssh-agent. Then we can skip the ```--private-key``` option in the ansible commands. 

### a) 
```
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/inf9380-2022-ssh  
``` 

### b) 
Now, go to the resources we will use which are here in the git repo you cloned earlier. 

``` cd $HOME/inf9380_cloudscripts/configure```
 
Inspect install_openmpi.yml and openmpi.sh You see that this is a playbook with the tasks directly written in the playbook. It installs some prerequisites for openmpi and openmpi itself. We also have to make sure that the system knows where to find openmpi, and we therefore have to add the location to the path. 

### c) 
You need to edit the inventory file "ansible_hosts" in the configure folder. It now contains some example host names and ip's. You need to change those according to your machines. 

You can get a list of the names and ip-s doing 

```openstack server list -c Networks -c Name | grep student<yournumber>```
where you change `<yournumber>` with your actual number, e.g. student40. 

copy paste the names and corresponding ip's to replace the ones in the ansible_hosts file. 

### d)
Now run:

 ``` 
ANSIBLE_HOST_KEY_CHECKING=False  ansible-playbook -i ansible_hosts install_openmpi.yml 
```
 
 
To be able to work smoothly, and not having to copy files from the master machine to the compute nodes, lets set up a shared filesystem. 
 
### e)
Inspect setup_nfs.yml and exportfs.j2 

Now run:

 ``` 
ANSIBLE_HOST_KEY_CHECKING=False  ansible-playbook -i ansible_hosts setup_nfs.yml 
```

You are done setting up your instances. 
 
## 9. Run a hello-world example with openmpi
 
Now we are ready to try mpirun running over all the 3 instances. 

All the 3 instances must have openmpi installed, and the hello world executable. We already installed openmpi on all the machines in step 7. 
We also set up a shared file system. The ```/scratch``` folder is exported from the admin machine to the two other instances. We will use that instead of copying the hello world executable to each machine with `scp`. 
  
On the admin machine:
 
```
cp $HOME/inf9380_cloudscripts/application/hello.f90 /scratch/
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

Now make a host-file so that mpi knows what machines to run on. 

Get a list of your servers:

```openstack server list | grep student<your-number>```

Copy the ipv4 adress into a text file in /scratch. Example:

```
cat /scratch/hosts.txt
158.39.48.1
158.39.200.2
158.37.63.3
```
 
 
Run the openmpi application
```
mpif90 -o hello.x hello.f90
mpirun -np 3  -hostfile hosts.txt hello.x
``` 

That should work! 

# All done
You have successfully created some resources (instances) in openstack with Terraform, you have configured the instances by installing some software and setting up shared filesystem with ansible, and you have run an openmpi application over the 3 machines in paralell. 

