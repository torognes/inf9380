# Install Personal HTCondor cluster

Install HTCondor on RHEL7/CentOS7
-----------------------------------
* After installing docker, install htcondor
```bash
# install wget
sudo yum install -y wget
# Configure the repos
cd /etc/yum.repos.d
sudo wget https://research.cs.wisc.edu/htcondor/yum/repo.d/htcondor-development-rhel7.repo
sudo wget https://research.cs.wisc.edu/htcondor/yum/repo.d/htcondor-stable-rhel7.repo
# Import signing key 
sudo wget http://research.cs.wisc.edu/htcondor/yum/RPM-GPG-KEY-HTCondor
sudo rpm --import RPM-GPG-KEY-HTCondor
# Install condor
sudo yum install condor.x86_64
# Start condor
sudo service condor start
```
* Verify that condor is up and running
```bash
$ ps -ef | grep condor
condor     22369       1  0 22:01 ?        00:00:00 /usr/sbin/condor_master -f
root       22412   22369  0 22:01 ?        00:00:00 condor_procd -A /var/run/condor/procd_pipe -L /var/log/condor/ProcLog -R 1000000 -S 60 -C 996
condor     22413   22369  0 22:01 ?        00:00:00 condor_shared_port -f
condor     22414   22369  0 22:01 ?        00:00:00 condor_collector -f
condor     22415   22369  0 22:01 ?        00:00:00 condor_negotiator -f
condor     22416   22369  0 22:01 ?        00:00:00 condor_schedd -f
condor     22417   22369  0 22:01 ?        00:00:00 condor_startd -f
condor     22459   22417  0 22:01 ?        00:00:00 kflops
cloud-u+   22461   21952  0 22:01 pts/0    00:00:00 grep --color=auto condor
[centos@norbis-35 docker_job]$ condor_status
Name                OpSys      Arch   State     Activity LoadAv Mem   ActvtyTime

norbis-35.novalocal LINUX      X86_64 Unclaimed Idle      0.000 3790  0+00:24:35

               Machines Owner Claimed Unclaimed Matched Preempting  Drain

  X86_64/LINUX        1     0       0         1       0          0      0

         Total        1     0       0         1       0          0      0
```
Use HTCondor
------------
* First set the following values in ``/etc/condor/condor_config``
```bash
CONDOR_HOST = $(IP_ADDRESS)
STARTER_ALLOW_RUNAS_OWNER = TRUE
TRUST_UID_DOMAIN=TRUE
```
* Then apply the new configuration to HTCondor
```bash
condor_reconfig
```
* Now submit a simple job
```bash
$ cat > job.sub
universe = vanilla
executable = /bin/date
output = job.out
error = job.err
queue
CTRL+D

$ condor_submit job.sub
Submitting job(s).
1 job(s) submitted to cluster 1.

$ cat job.out
Sun Jan 22 11:05:40 UTC 2017
```
* Submit a job with a simple executable
  * make the executable:
  ```bash
  $ cat > think.sh
  echo thinking really hard for $1 seconds ...
  sleep $1
  echo Done!
  ```
  * Try it out:
  ```bash
  $ sh think.sh 5
  thinking really hard for 5 seconds ...
  Done!
  ```
  * Now create the job and submit it:
  ```bash
  $ cat > think.sub
  universe = vanilla
  executable = /usr/bin/sh
  arguments = think.sh 30
  input = think.sh
  output = job.out
  error = job.err
  queue
  $ condor_submit think.sub
  Submitting job(s).
  1 job(s) submitted to cluster 12.
  $ condor_q


  -- Schedd: norbis-35.novalocal : <158.37.63.80:9618?... @ 04/23/18 10:35:21
  OWNER  BATCH_NAME    SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
  centos ID: 11       4/23 10:35      _      _      1      1 11.0

  Total for query: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  Total for centos: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  Total for all users: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  ```
  * Now check your output file:
  ```bash
  $ cat job.out
  thinking really hard for 30 seconds ...
  Done!
  ```
* Use the log file:
  * Add this line to the ``think.sub`` file: ``log = job.log``
  * Submit again:
  ```bash
  condor_submit think.sub
  Submitting job(s).
  1 job(s) submitted to cluster 12.
  $ condor_q


  -- Schedd: norbis-35.novalocal : <158.37.63.80:9618?... @ 04/23/18 10:35:21
  OWNER  BATCH_NAME    SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
  centos ID: 11       4/23 10:35      _      _      1      1 11.0

  Total for query: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  Total for centos: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  Total for all users: 1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
  ```
  * Now check your log file:
  ```bash
  $ cat job.log
  000 (005.000.000) 04/23 08:00:44 Job submitted from host: <158.37.63.80:9618?addrs=158.37.63.80-9618+[2001-700-2-8200--2106]-9618&noUDP&sock=19042_ce7f_4>
  ...
  001 (005.000.000) 04/23 08:00:44 Job executing on host: <158.37.63.80:9618?addrs=158.37.63.80-9618+[2001-700-2-8200--2106]-9618&noUDP&sock=19042_ce7f_5>
  ...
  006 (005.000.000) 04/23 08:00:53 Image size of job updated: 1000
    1  -  MemoryUsage of job (MB)
    284  -  ResidentSetSize of job (KB)
  ...
  005 (005.000.000) 04/23 08:01:14 Job terminated.
    (1) Normal termination (return value 0)
      Usr 0 00:00:00, Sys 0 00:00:00  -  Run Remote Usage
      Usr 0 00:00:00, Sys 0 00:00:00  -  Run Local Usage
      Usr 0 00:00:00, Sys 0 00:00:00  -  Total Remote Usage
      Usr 0 00:00:00, Sys 0 00:00:00  -  Total Local Usage
    0  -  Run Bytes Sent By Job
    0  -  Run Bytes Received By Job
    0  -  Total Bytes Sent By Job
    0  -  Total Bytes Received By Job
    Partitionable Resources :    Usage  Request Allocated
       Cpus                 :        0        1         1
       Disk (KB)            :     1000     1000  19655560
       Memory (MB)          :        1        1      3790
  ...
  ```
* Submit a job to the docker universe
```bash
$ cat > docker_job.sub
universe = docker
docker_image = ubuntu
executable = /bin/cat
arguments = /etc/os-release
output = docker_job.out
error = docker_job.err
queue
ctrl+D

$ condor_submit docker_job.sub
Submitting job(s).
1 job(s) submitted to cluster 12.

$ condor_q


-- Schedd: docker-test.novalocal : <192.168.1.126:9618?... @ 05/28/17 23:10:40
OWNER      BATCH_NAME       SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
cloud-user CMD: /bin/cat   5/28 23:10      _      1      _      1 12.0

1 jobs; 0 completed, 0 removed, 0 idle, 1 running, 0 held, 0 suspended

$ cat docker_job.out
NAME="Ubuntu"
VERSION="16.04.2 LTS (Xenial Xerus)"
ID=ubuntu
ID_LIKE=debian
PRETTY_NAME="Ubuntu 16.04.2 LTS"
VERSION_ID="16.04"
HOME_URL="http://www.ubuntu.com/"
SUPPORT_URL="http://help.ubuntu.com/"
BUG_REPORT_URL="http://bugs.launchpad.net/ubuntu/"
VERSION_CODENAME=xenial
UBUNTU_CODENAME=xenial
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

Useful links
-------------
* [Install Condor](https://research.cs.wisc.edu/htcondor/instructions/el/7/stable/)
* [Configure Condor for multiple nodes](https://spinningmatt.wordpress.com/2011/06/12/getting-started-creating-a-multiple-node-condor-pool/)
* [Docker universe in Condor](https://research.cs.wisc.edu/htcondor/manual/latest/3_16Setting_Up.html#SECTION004162000000000000000)
* [Condor security](http://research.cs.wisc.edu/htcondor/manual/v8.2/3_6Security.html)
