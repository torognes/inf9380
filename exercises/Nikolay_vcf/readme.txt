First, enable the latest g++ compiler using:

module load gcc/5.3.0

Next run the make.sh script.
Finally run the "find_novel_snps" program or the "scr.sh" command with "time" to measure the time for 100 runs of the program. In either case appending the argument "u" runs the program in no-threading mode. Appending an integer (>0) instead runs the program with the corresponding number of threads. E.g:

time ./scr.sh u
time ./scr.sh 4

Also, appending the names of the subject and control to the command will change the subject and control .vcf files from the default to the user specified. E.g:

time ./scr.sh 4 subject.vcf control.vcf

will run the program with 4 cores using the subject.vcf as the target and control.vcf as the control snp lists. NB! Order matters! Finally the program saves the snps found to the file "differences.vcf".

