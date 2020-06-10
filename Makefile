COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app -Wall -O3
run:
	build/app data/test.c 
