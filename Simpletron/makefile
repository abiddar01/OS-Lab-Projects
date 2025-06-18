simple.exe: simple.o libsimple.a
	gcc -o simple.exe simple.o -L. -lsimple

simple.o: simple.c simple.h
	gcc -c simple.c

util.o: util.c
	gcc -c util.c

libsimple.a: util.o
	ar cr libsimple.a util.o

clean: 
	rm *.o libsimple.a simple

