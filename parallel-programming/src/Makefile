all : parallel serial

clean :
	rm parallel serial gmon.out

serial : serial.c Makefile
	gcc -Wall -g -o serial serial.c

parallel : parallel.c Makefile
	gcc -Wall -g -pthread -o parallel parallel.c
