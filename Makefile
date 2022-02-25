vupl: parse.o execute.o file.o main.o
	gcc -o vupl parse.o execute.o file.o main.o

parse.o: src/parse.c
	gcc -c -o parse.o src/parse.c

execute.o: src/execute.c
	gcc -c -o execute.o src/execute.c

file.o: src/file.c
	gcc -c -o file.o src/file.c

main.o: src/main.c
	gcc -c -o main.o src/main.c
