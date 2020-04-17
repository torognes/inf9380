Please refer to the tutorial instructions [part 1](https://github.com/torognes/inf9380/blob/master/cloud/elasticluster_part1.md) and  [part 2](https://github.com/torognes/inf9380/blob/master/cloud/elasticluster_part2.md) for all initial set up, where to find your login information, how to log into the admin machine, and how to use elasticluster. 


## How to delete a cluster
You must first activate the elasticluster environment, then you can delete the cluster. Here is an example showing the deletion of the cluster student00. Please use your specific cluster name. 



```
cd $HOME
. elasticluster/bin/activate
elasticluster stop student00
```
## Install the exam version of Elasticluster on admin server
Note: updated information about version will come here. 

We will install the latest version of Elasticluster following the installation instructions from the [Elasticluster docs](https://elasticluster.readthedocs.io/en/latest/install.html#installing-development-code-from-github)

If you followed the course tutorial you can skip the first line below. 

```
sudo yum install -y gcc gcc-c++ git libffi-devel openssl-devel python3 python3-devel python3-virtualenv
virtualenv-3 elasticluster
. elasticluster/bin/activate
cd elasticluster
git clone git://github.com/gc3-uzh-ch/elasticluster.git src
cd src
pip3 install -e .
cd
```

