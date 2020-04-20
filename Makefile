shell:shell.o cat.o ls.o
	gcc shell.o cat.o ls.o -o shell
shell.o:shell.c Header.h cat.h ls.h
	gcc -c shell.c
cat.o:cat.c Header.h cat.h
	gcc -c cat.c
ls.o:ls.c Header.h ls.h 
	gcc -c ls.c