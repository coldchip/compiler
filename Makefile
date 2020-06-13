COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app -Wall -O3
run:
	build/app data/test.c 
test:
	valgrind --leak-check=yes -s build/app data/test.c
