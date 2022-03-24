# Day 1 script
cat /proc/cpuinfo | grep processor

cd /home/centos/bioinfo/data

fastqc -t 2 SRR2584863_R1.fq.gz SRR2584863_R2.fq.gz

java -jar trimmomatic-0.38.jar -threads 4 PE SRR2584863_R1.fq.gz SRR2584863_R2.fq.gz \
output_forward_paired.fq.gz output_forward_unpaired.fq.gz \
output_reverse_paired.fq.gz output_reverse_unpaired.fq.gz \
ILLUMINACLIP:ref/adapter/NexteraPE-PE.fa:2:30:10 \
LEADING:3 TRAILING:3 SLIDINGWINDOW:4:15 MINLEN:25 

cd ref
bwa index ecoli_rel606.fa

cd /home/centos/bioinfo/data
bwa mem -t 4 ref/ecoli_rel606.fa output_forward_paired.fq.gz output_reverse_paired.fq.gz > output_paired.sam
samtools view -S -b output_paired.sam > output_paired.bam
samtools sort -o output_paired.sorted.bam output_paired.bam
samtools flagstat output_paired.sorted.bam > output_paired.sorted.bam_flagstat


bcftools mpileup -O b -o output_paired.raw.bcf -f ref/ecoli_rel606.fa output_paired.sorted.bam
bcftools call --ploidy 1 -m -v -o  output_paired.variants.vcf output_paired.raw.bcf 

# Day 2 script

cd /home/centos/
nextflow run hello
cd /home/centos/.nextflow/assets/nextflow-io/hello
# Check main.nf and nextflow.config using $ cat

cd /home/centos/bioinfo/nextflow
# Check main.nf, modules.nf nextflow.config using $ cat
nextflow run main.nf
# Check output and results/ folder
# Transfer the *html files and check them

# Add "-t $task.cpus" to FASTQC and BWA 
# Add "-threads $task.cpus" to TRIMMOMATIC
nextflow run main.nf
# Check output and results/ folder
# Transfer the *html files and check them


## Day 23 script

cd /home/centos
nextflow run hello -with-docker

cd /home/centos/bioinfo/nextflow_docker/
nextflow run main.nf

nextflow run main.nf -with-docker arvindsundaram/vc_norbis

# Create docker with the norbis_vc_t2020.dockerfile
docker build -t norbis_vc:2022 - < norbis_vc_t2020.dockerfile
nextflow run main.nf -with-docker norbis_vc_t2020.dockerfile

# Use the following containers to nextflow.config:

container = "staphb/fastqc"
container = "staphb/trimmomatic"
container = "staphb/fastqc"
container = "dukegcb/bwa-samtools"
container = "dukegcb/bwa-samtools"
container = "dceoy/bcftools"

# Add the following line to nextflow.config:

docker.enabled = true


nextflow run main.nf