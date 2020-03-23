# Getting set up
An admin machine has been created for each of you. This is a linux CentOS 7 virtual machine on the NREC Openstack resources. We will use this machine for the cloud cluster hands-on session, so that you all have the same setup. 


## 1. Download the ssh key we will use to your laptop
If you are on linux: 
```
cd .ssh
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/inf9380-ssh
chmod 0600 ~/.ssh/inf9380-ssh
```

If you are using PuTTY, follow the procedures you know for PuTTY (e.g. like this: https://support.rackspace.com/how-to/log-into-a-linux-server-with-an-ssh-private-key-on-windows/)


## 2. Log into your admin server
Your username on the admin machine is the default username centos
Find the machine assigned to you from this list: <...> 
On linux do: 
```
ssh -i .ssh/inf9380-ssh centos@admin-studentNN
```
On Windows, use PuTTY to login


## 3. Install Elasticluster on the admin server
We install the latest version of Elasticluster following the installation instructions from the [Elasticluster docs](https://elasticluster.readthedocs.io/en/latest/install.html#installing-development-code-from-github)


```
sudo yum install gcc gcc-c++ git libffi-devel openssl-devel python-devel python-virtualenv
virtualenv elasticluster
. elasticluster/bin/activate
pip install --upgrade 'pip>=9.0.0'
pip install 'setuptools>=40.3.0'
cd elasticluster
git clone git://github.com/gc3-uzh-ch/elasticluster.git src
cd src
pip install -e .
```

## 4. Set up example config file
https://elasticluster.readthedocs.io/en/latest/configure.html
```
elasticluster list-templates
```
This will throw some errors, but create an example configuration file in ~/.elasticluster/config


## 5. Prepare access to NREC (Openstack) for Elasticluster to work
Elasticluster contacts NREC's Openstack API and in order to allow this, you  must authenticate yourself. We store our authentication information in a file with environment variables that are used by Openstack.

* Set up your authentication environment to NREC
   * Download the keystone.sh file into your home directory
  
```
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/keystone.sh
```
Open the file and enter your NREC username and password which you find here: < ... >


```
source keystone.sh
```
Check that the variables were exported

```
printenv | grep OS_ 
```


## 6. Get the ssh key-pairs that we will use to log into the cluster
```
cd ~/.ssh
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/inf9380-ssh
chmod 0600 inf9380-ssh
wget https://raw.githubusercontent.com/torognes/inf9380/master/cloud/inf9380-ssh.pub
```

You are all set to work with Elasticluster!

