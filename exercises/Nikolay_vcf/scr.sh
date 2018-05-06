#!/bin/bash
for run in {1..100} 
do 
    ./find_novel_snps $1 $2 $3
done
