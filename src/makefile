make : *.c
	gcc -Wall -c *.c
	gcc -o exec *.o -L../lib -lSDL2
	valgrind ./exec

clean : *.o exec
	rm -rf *.o exec

