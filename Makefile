COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app -Wall -g
run:
	build/app data/test.c 
test:
	valgrind --leak-check=yes -s build/app data/test.c
