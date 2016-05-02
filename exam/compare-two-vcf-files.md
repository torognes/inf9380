#Compare two VCF files and report the differences (Everybody except those who completed it)

Remember the last step in variant calling workflow? here it is:

7 Compare the subject to the control
-------------------------------------
Now we have the final results for both our subject and our control. However, what we really
want to look at, are the SNPs that are there in the subject, but not in the control. Make a
python scripts that does this with an option to parallelize. 

You should write a python script that compare the two vcf files and store the difference in a third vcf. 
* Create a serial code
* Create a parallel code
* report the overall time (both ``real`` and ``user`` time) for each

