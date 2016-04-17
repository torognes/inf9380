all : parallel serial

clean :
	rm parallel serial gmon.out

serial : serial.c Makefile
	gcc -o serial serial.c -Wall -g

parallel : parallel.c Makefile
	gcc -o parallel parallel.c -Wall -g -pthread
