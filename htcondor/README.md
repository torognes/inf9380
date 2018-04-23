# Install Personal HTCondor cluster

Install HTCondor on RHEL7/CentOS7
-----------------------------------
* Since we are planning to use HTCondor with Docker, first you need to [install docker](https://github.com/torognes/inf9380/blob/master/docker/install-docker.md)
* Install HTCondor
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
# Enable condor to run docker
sudo usermod -aG docker condor
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
* Verify that condor detects that docker is installed
```bash
$ condor_status -l | grep Docker
DockerVersion = "Docker version 17.03.1-ce, build c6d412e"
HasDocker = true
StarterAbilityList = "HasDocker,HasFileTransfer,HasTDP,HasPerFileEncryption,HasVM,HasReconnect,HasMPI,HasFileTransferPluginMethods,HasJobDeferral,HasJICLocalStdin,HasJICLocalConfig"
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
  1. make the executable:
  ```bash
  $ cat > think.sh
  echo thinking really hard for $1 seconds ...
  sleep $1
  echo Done!
  ```
  2. Try it out:
  ```bash
  $ sh think.sh 5
  thinking really hard for 5 seconds ...
  Done!
  ```
  3. Now create the job and submit it:
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
  4. Now check your output file:
  ```bash
  $ cat job.out
  thinking really hard for 30 seconds ...
  Done!
  ```
* Use the log file:
  1. Add this line to the ``think.sub`` file: ``log = job.log``
  2. Submit again:
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
  3. Now check your log file:
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
Python job on HTCondor
-----------------------
* Submit a simple python job that reads a file and writes an output
  1. Create a simple input file:
  ```bash
  $ cat > in.txt
  This is input text file!
  ```
  2. Create the python script:
  ```python
  $ cat readwrite.py
  import sys
  import time
  f = open(sys.argv[1],'r')
  input = f.read()

  print 'This is the input line:\n'+ 20*'='+'\n' + input + 20*'='

  print 'Thinking for ' + sys.argv[2] + ' seconds...'
  time.sleep(int(sys.argv[2]))

  print '\nNow we are done! :)'
  ```
  3. Try the script:
  ```bash
  $ python readwrite.py in.txt 5
  This is the input line:
  ====================
  This is input text file!
  ====================
  Thinking for 5 seconds...

  Now we are done! :)
  ```
  4. Now create the job submission script:
  ```bash
  $ cat pyjob.sub
  universe = vanilla
  executable = /usr/bin/python
  arguments = readwrite.py in.txt 30
  transfer_input_files = readwrite.py, in.txt
  output = job.out
  log = job.log
  error = job.err
  queue
  ```
  5. Submit your job and see the output:
  ```bash
  $ condor_submit pyjob.sub
  Submitting job(s).
  1 job(s) submitted to cluster 12.
  ....
  $ cat job.out
  This is the input line:
  ====================
  This is input text file!
  ====================
  Thinking for 30 seconds...

  Now we are done! :)
  ```
  6. Let's modify the python script to write the output to a file:
  ```bash
  $ cat > readwrite2.py
  import sys
  import time
  f = open(sys.argv[1],'r')
  input = f.read()

  fw = open ('out.txt','w')

  print >> fw, 'This is the input line:\n'+ 20*'='+'\n' + input + 20*'='

  print 'Thinking for ' + sys.argv[2] + ' seconds...'
  time.sleep(int(sys.argv[2]))

  print '\nNow we are done! :)'
  ```
  7. Now modify the submission script to capture the output file:
  ```bash
  $ cat pyjob2.sub
  universe = vanilla
  executable = /usr/bin/python
  arguments = readwrite2.py in.txt 30
  transfer_input_files = readwrite.py, in.txt
  transfer_output_files = out.txt
  output = job.out
  log = job.log
  error = job.err
  queue
  ```
  8. Submit the new job and check both the standard output and the job output files, ``job.out`` and ``out.txt``:
  ```bash
  $ condor_submit pyjob.sub
  Submitting job(s).
  1 job(s) submitted to cluster 13.
  .....
  $ cat job.out
  Thinking for 30 seconds...

  Now we are done! :)
  $ cat out.txt
  This is the input line:
  ====================
  This is input text file!
  ====================
  ```
* Create a parallel HTCondor job
  1. Generate 3 input files:
  ```bash
  $ cat > in0.txt
  This is input text file 0!
  $ cat > in1.txt
  This is input text file 1!
  $ cat > in2.txt
  This is input text file 2!
  ```
  2. Create a new python script:
  ```python
  $ cat > readwrite_many.py
  import sys
  import time

  # The file index
  index = sys.argv[1]

  f = open('in'+index+'.txt','r')
  input = f.read()

  fw = open ('out'+index+'.txt','w')

  print >> fw, 'This is the input line:\n'+ 20*'='+'\n' + input + 20*'='

  print 'Thinking for ' + sys.argv[2] + ' seconds...'
  time.sleep(int(sys.argv[2]))

  print '\nNow we are done! :)'
  ```
  3. Create a submission script:
  ```bash
  $ cat > pyjob_many.sub
  universe = vanilla
  executable = /usr/bin/python
  arguments = readwrite_many.py $(process) $(process)
  transfer_input_files = readwrite_many.py, in$(process).txt
  transfer_output_files = out$(process).txt
  output = job$(process).out
  log = job.log
  error = job$(process).err
  queue 3
  ```
  4. Now submit the job and check out ``out0.txt``, ``out1.txt``, and ``out2.txt``. Also check out ``job0.out``,``job1.out``, and ``job2.out``
  
Useful links
-------------
* [Install Condor](https://research.cs.wisc.edu/htcondor/instructions/el/7/stable/)
* [Configure Condor for multiple nodes](https://spinningmatt.wordpress.com/2011/06/12/getting-started-creating-a-multiple-node-condor-pool/)
* [Docker universe in Condor](https://research.cs.wisc.edu/htcondor/manual/latest/3_16Setting_Up.html#SECTION004162000000000000000)
* [Condor security](http://research.cs.wisc.edu/htcondor/manual/v8.2/3_6Security.html)
