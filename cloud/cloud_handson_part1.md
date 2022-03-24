```
sudo -i
dnf install -y dnf-utils
dnf config-manager --add-repo https://rpm.releases.hashicorp.com/RHEL/hashicorp.repo
dnf install -y terraform


dnf install -y centos-release-openstack-ussuri
dnf config-manager --set-enabled powertools
dnf install -y python3-openstackclient
dnf install -y openstack-selinux
