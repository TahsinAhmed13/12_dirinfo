main: main.c
	gcc -o main main.c

.PHONY: run

run: main
	./main
