# Day 1: https://bit.ly/3MUHKoh

# Day 2: https://bit.ly/3MVi1fy

# Day 3: https://bit.ly/3tljF2l


## Day 1
# Login to NREC CentOS 7

sudo yum -y install java perl unzip bzip2 vim
sudo yum -y install zlib-devel bzip2-devel xz-devel ncurses-devel
sudo yum -y install gcc

cd /home/centos/
mkdir bioinfo
cd bioinfo
curl "https://filesender2.uio.no/download.php?token=b2578b88-8b48-4c93-aac9-bb157642b927&files_ids=155667" > data.tar
tar -xvf data.tar
curl "https://filesender2.uio.no/download.php?token=b2578b88-8b48-4c93-aac9-bb157642b927&files_ids=155666" > tools.tar
tar -xvf tools.tar

# Install FastQC
cd /home/centos/bioinfo/tools
unzip fastqc_v0.11.9.zip
cd FastQC
chmod +x fastqc
sudo ln -s /home/centos/bioinfo/tools/FastQC/fastqc /usr/local/bin/

# Install Trimmomatic
cd /home/centos/bioinfo/tools
unzip Trimmomatic-0.36.zip
cd Trimmomatic-0.36
chmod +x trimmomatic-0.36.jar

# Install bwa
cd /home/centos/bioinfo/tools
tar -xvf bwa-0.7.17.tar.bz2
cd bwa-0.7.17
make
sudo ln -s /home/centos/bioinfo/tools/bwa-0.7.17/bwa /usr/local/bin/
sudo ln -s /home/centos/bioinfo/tools/bwa-0.7.17/qualfa2fq.pl /usr/local/bin/
sudo ln -s /home/centos/bioinfo/tools/bwa-0.7.17/xa2multi.pl /usr/local/bin/

# Install bcftools
cd /home/centos/bioinfo/tools
tar -xvf bcftools-1.15.tar.bz2
cd bcftools-1.15
./configure --prefix=/usr/local/
make
sudo make install

# Install samtools
cd /home/centos/bioinfo/tools
tar -xvf samtools-1.15.tar.bz2
cd samtools-1.15
./configure --prefix=/usr/local/
make
sudo make install



## Day 2

# Install nextflow
cd /home/centos/bioinfo/tools
curl -s https://get.nextflow.io | bash
sudo ln -s /home/centos/bioinfo/tools/nextflow /usr/local/bin/

cd /home/centos/bioinfo/
curl "https://filesender2.uio.no/download.php?token=f25e6033-6919-4f4f-b33e-540d27fe9cd1&files_ids=155824" > nextflow.tar
tar -xvf nextflow.tar

# Day 3
cd /home/centos/bioinfo/
curl "https://filesender2.uio.no/download.php?token=6dcace0a-98f8-487f-a3b6-d447326b1a85&files_ids=156103" > nextflow_docker.tar
tar -xvf nextflow_docker.tar

