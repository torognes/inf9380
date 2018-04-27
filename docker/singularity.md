# Singularity

Installation
-------------
* Install the development tools:
```bash
sudo yum groupinstall "development tools"
```
* Install the release of your choice. The releases page is [here](https://github.com/singularityware/singularity/releases)
```bash
VERSION=2.4.6
wget https://github.com/singularityware/singularity/releases/download/$VERSION/singularity-$VERSION.tar.gz
tar xvf singularity-$VERSION.tar.gz
cd singularity-$VERSION
./configure --prefix=/usr/local
make
sudo make install

```
* Create a simlink for root
```bash
sudo ln -s /usr/local/bin/singularity /usr/bin/singularity 
```
* Install squashfs-tools
```bash
sudo yum install squashfs-tools
```

Getting started
----------------
* See the interface
```bash
$ singularity --help
USAGE: singularity [global options...] <command> [command options...] ...

GLOBAL OPTIONS:
    -d|--debug    Print debugging information
    -h|--help     Display usage summary
    -s|--silent   Only print errors
    -q|--quiet    Suppress all normal output
       --version  Show application version
    -v|--verbose  Increase verbosity +1
    -x|--sh-debug Print shell wrapper debugging information

GENERAL COMMANDS:
    help       Show additional help for a command or container                  
    selftest   Run some self tests for singularity install                      

CONTAINER USAGE COMMANDS:
    exec       Execute a command within container                               
    run        Launch a runscript within container                              
    shell      Run a Bourne shell within container                              
    test       Launch a testscript within container                             

CONTAINER MANAGEMENT COMMANDS:
    apps       List available apps within a container                           
    bootstrap  *Deprecated* use build instead                                   
    build      Build a new Singularity container                                
    check      Perform container lint checks                                    
    inspect    Display a container's metadata                                   
    mount      Mount a Singularity container image                              
    pull       Pull a Singularity/Docker container to $PWD                      

COMMAND GROUPS:
    image      Container image command group                                    
    instance   Persistent instance command group                                


CONTAINER USAGE OPTIONS:
    see singularity help <command>

For any additional help or support visit the Singularity
website: http://singularity.lbl.gov/
```
* Get help
```bash
$ singularity help <subcommand>
$ singularity --help <subcommand>
$ singularity -h <subcommand>
$ singularity <subcommand> --help
$ singularity <subcommand -h
```
* Download pre-built images

From singularity hub:
```bash
$ singularity pull shub://vsoch/hello-world   # pull with default name, vsoch-hello-world-master.simg
$ singularity pull --name hello.simg shub://vsoch/hello-world   # pull with custom name
```
From Docker hub:
```bash
$ singularity pull docker://godlovedc/lolcow  # with default name
$ singularity pull --name funny.simg docker://godlovedc/lolcow # with custom name
```
* Build images
```bash
$ singularity build hello-world.simg shub://vsoch/hello-world
$ singularity build lolcow.simg docker://godlovedc/lolcow
```
* Interact with containers
```bash
$ cat /etc/redhat-release 
CentOS Linux release 7.2.1511 (Core) 
$ singularity shell docker://ubuntu:latest
library/ubuntu:latest
Downloading layer: sha256:a3ed95caeb02ffe68cdd9fd84406680ae93d633cb16422d00e8a7c22955b46d4
Downloading layer: sha256:9f03ce1741bf604c84258a4c4f1dc98cc35aebdd76c14ed4ffeb6bc3584c1f9b
Downloading layer: sha256:61e032b8f2cb04e7a2d4efa83eb6837c6b92bd1553cbe46cffa76121091d8301
Downloading layer: sha256:50de990d7957c304603ac78d094f3acf634c1261a3a5a89229fa81d18cdb7945
Downloading layer: sha256:3a80a22fea63572c387efb1943e6095587f9ea8343af129934d4c81e593374a4
Downloading layer: sha256:cad964aed91d2ace084302c587dfc502b5869c5b1d15a1f0e458a45e3cadfaa6
Singularity: Invoking an interactive shell within container...

Singularity.ubuntu:latest> cat /etc/lsb-release
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=16.04
DISTRIB_CODENAME=xenial
DISTRIB_DESCRIPTION="Ubuntu 16.04.1 LTS"
Singularity.ubuntu:latest> which apt-get
/usr/bin/apt-get
Singularity.ubuntu:latest> exit
```
* Interact with a pulled image
```bash
$ singularity pull --name hello-world.simg shub://vsoch/hello-world
$ singularity shell hello-world.simg
Singularity: Invoking an interactive shell within container...

# I am the same user inside as outside!
Singularity hello-world.simg:~/Desktop> whoami
vanessa

Singularity hello-world.simg:~/Desktop> id
uid=1000(vanessa) gid=1000(vanessa) groups=1000(vanessa),4(adm),24,27,30(tape),46,113,128,999(input)
```
* Execute a command on a pulled image
```bash
$ singularity exec hello-world.simg ls /
anaconda-post.log  etc	 lib64	     mnt   root  singularity  tmp
bin		   home  lost+found  opt   run	 srv	      usr
dev		   lib	 media	     proc  sbin  sys	      var
```
``exec`` also works with the ``shub://`` and ``docker://`` URIs. This creates an ephemeral container that executes a command and disappears.
```bash
$ singularity exec shub://singularityhub/ubuntu cat /etc/os-release
```

Singularity recipes
------------------
Read the docs [here](http://singularity.lbl.gov/quickstart#singularity-recipes)

Docker example:

* Create a ubuntu (from docker) def file
```bash
cat >> ubuntu.def
BootStrap: docker
From: ubuntu:latest

%runscript
    echo "This is what happens when you run the container..."
    

%post
    echo "Hello from the container"
    echo "Install additional software here"
    # This part is for Abel and colossus clusters at UiO:
    mkdir /cluster /work /usit /tsd /net /projects
```
* Build the container from the def file
```bash
sudo singularity build ubuntu.img ubuntu.def
```
Use a Singularity container on Abel/Colossus
---------------------------------------------
On your VM:

```bash
docker search bowtie2
...
sudo singularity build -w bowtie2.simg docker://biocontainers/bowtie2
sudo singularity shell -w bowtie2.simg
Singularity bowtie2.simg:/root> mkdir /cluster /work /usit /tsd /net /projects
Singularity bowtie2.simg:/root> exit

scp bowtie2.simg <abel-user>@abel.uio.no:~/
```
Now go to Abel:

```bash
ssh <abel-user>@abel.uio.no
[<abel-user>@login-0-1 ~]$ module avail singularity
---------------------------------------------- /cluster/etc/modulefiles -----------------------------------------------
singularity/2.2.1          singularity/2.4.2          singularity/2.4.5(default)
singularity/2.4            singularity/2.4.4
[<abel-user>@login-0-1 ~]$ module load singularity
[<abel-user>@login-0-1 ~]$ qlogin -A <project>
salloc: Pending job allocation 20627631
salloc: job 20627631 queued and waiting for resources
salloc: job 20627631 has been allocated resources
salloc: Granted job allocation 20627631
srun: Job step created
bash-4.1$ singularity exec bowtie2.simg bowtie2 --version
/home/biodocker/bin/bowtie2-align-s version 2.2.9
64-bit
Built on localhost.localdomain
Thu Apr 21 18:36:37 EDT 2016
Compiler: gcc version 4.1.2 20080704 (Red Hat 4.1.2-54)
Options: -O3 -m64 -msse2  -funroll-loops -g3 -DPOPCNT_CAPABILITY
Sizeof {int, long, long long, void*, size_t, off_t}: {4, 8, 8, 8, 8, 8}
bash-4.1$
```
* Submit a [test singularity job](https://github.com/abdulrahmanazab/Docker-Training-NeIC2017/blob/master/singularity-slurm-abel.sh) to slurm on abel

More ...
-------------
* [File sharing](http://singularity.lbl.gov/docs-mount)

Useful links
-------------
* [Intro video](http://singularity.lbl.gov/2015-singularity-intro-video)
