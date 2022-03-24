Please refer to the tutorial instructions [part 1](https://github.com/torognes/inf9380/blob/master/cloud/elasticluster_part1.md) and  [part 2](https://github.com/torognes/inf9380/blob/master/cloud/elasticluster_part2.md) for all initial set up, where to find your login information, how to log into the admin machine, and how to use elasticluster. 



##  Erratum to exam desription
Image_id changes frequently unfortunately due to updates at NREC. If you get an error message in elasticluster related to image_id it is probably because you are using an image_id which by now is outdated. We have been updating a couple of times already. The new active image as os 2020-05-04 is
```
image_id=61e6ef20-0e58-473e-9056-1fb65d17989d
```

## How to delete a cluster
You must first activate the elasticluster environment, then you can delete the cluster. Here is an example showing the deletion of the cluster student00. Please use your specific cluster name. 



```
cd $HOME
. elasticluster/bin/activate
elasticluster stop student00
```
## Install the exam version of Elasticluster on admin server

We will use Maiken's fork of Elasticluster as of 2020-04-17: https://github.com/maikenp/elasticluster.git
If you compare the below instructions with the installation instructions in the course tutorial the sequence is the same, the only difference is the link to the source repository. 


If you followed the course tutorial you can skip the first line below. 

```
sudo yum install -y gcc gcc-c++ git libffi-devel openssl-devel python3 python3-devel python3-virtualenv
virtualenv-3 elasticluster
. elasticluster/bin/activate
cd elasticluster
git clone https://github.com/maikenp/elasticluster.git src
cd src
pip3 install -e .
cd
```

