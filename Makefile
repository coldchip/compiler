COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app.exe -lws2_32 -Wall -O3
run:
	build/app.exe data/test.c 
