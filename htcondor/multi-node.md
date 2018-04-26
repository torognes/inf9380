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
HAS_R = TRUE
STARTD_EXPRS = HAS_R
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
Useful material
----------------
[Tutorial](http://research.cs.wisc.edu/htcondor/tutorials/fermi-2005/)
