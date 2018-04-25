#!/bin/sh

for num in {1..100000}  
	do echo $num> $num.txt
done
for num in {1..100000}
	do cat $num.txt >> all.txt
done
