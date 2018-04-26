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
