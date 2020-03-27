COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app.exe -lws2_32 -Wall