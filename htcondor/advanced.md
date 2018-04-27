# Advanced HTCondor setup

Custom machine classad attributes
----------------------------------
You may configure custom classad attributes. An example:

* Install R:
```bash
sudo yum install epel-release -y
sudo yum install R -y
```
* Add the following to ``/etc/condor/condor_config``:
```bash
Has_R = TRUE
STARTD_EXPRS = Has_R
```
* Reconfigure condor, then test the new attribute:
```bash
$ condor_reconfig
Sent "Reconfig" command to local master
$ condor_status -constraint "Has_R == TRUE"
Name                      OpSys      Arch   State     Activity LoadAv Mem   ActvtyTime

condor-docker-1.novalocal LINUX      X86_64 Unclaimed Idle      0.000  968  0+01:34:57

               Machines Owner Claimed Unclaimed Matched Preempting  Drain

  X86_64/LINUX        1     0       0         1       0          0      0

         Total        1     0       0         1       0          0      0
```
Install a multi-node cluster
-----------------------------
* On the broker node, add:
```bash
ALLOW_WRITE = $(ALLOW_WRITE),$(IP_ADDRESS), <reg-node1-IPAddress>, <reg-node2-IPAddress>, ...
```
* On the regular nodes config file, add:
```bash
CONDOR_HOST = <broker-IPAddress>
ALLOW_WRITE = $(ALLOW_WRITE), $(CONDOR_HOST)
```
Shared disk between VMs using sshfs
------------------------------------
This is how to configure a shared VM to have a shared disk:
As root:
- create a directory to share: ``/local-shared``
- set ``PasswordAuthentication yes`` in ``/etc/ssh/sshd_config``
- Restart the sshd service: ``service sshd restart``
- set a password for root

Now in Your VM (as root):
* ``yum install sshfs``
* uncomment “user_allow_other” in ``/etc/fuse.conf``
* Create a directory to mount the shared area and attach the volume ``sshfs``:
```bash
mkdir /shared
chmod +rw /shared
sshfs -o allow_other,auto_unmount root@<shared-vm IP>:/local-shared /shared
```
*Use the root password for the shared VM*

Now all users will have access to /shared 

Useful info:
* To unmount: ``fusermount -u /shared``
* To activate auto-mount the shared disk at startup: 
Add the following line to ``/etc/crontab``: 
```bash
@reboot sshfs -o allow_other,auto_unmount root@<shared-vm IP>:/local-shared /shared 
```
HTCondor Docker job with mounted volumes
---------------------------------------
* Create directories for input/output volumes
```bash
mkdir docker_in
echo hello! > docker_in/infile
mkdir docker_out
```
* Configure Condor to mount volumes on Docker images, as root:
```bash
$ sudo cat > /etc/condor/config.d/docker
#Define volumes to mount:
DOCKER_VOLUMES = DOCKER_IN, DOCKER_OUT

#Define a mount point for each volume:
DOCKER_VOLUME_DIR_DOCKER_IN = /home/cloud-user/docker_in:/input:ro
DOCKER_VOLUME_DIR_DOCKER_OUT = /home/cloud-user/docker_out:/output:rw

#Configure those volumes to be mounted on each Docker container:
DOCKER_MOUNT_VOLUMES = DOCKER_IN, DOCKER_OUT
ctrl+D
```
* Create the submission script and submit the job
```bash
$ cat > docker_volumes_job.sub
universe = docker
docker_image = centos
executable = /bin/cp
arguments = /input/infile /output/outfile
output = docker_volumes_job.out
error = docker_volumes_job.err
queue
ctrl+D

$ condor_submit docker_volumes_job.sub
Submitting job(s).
1 job(s) submitted to cluster 7.
$ cat docker_out/outfile
hello!
```
Useful material
----------------
[Tutorial](http://research.cs.wisc.edu/htcondor/tutorials/fermi-2005/)
