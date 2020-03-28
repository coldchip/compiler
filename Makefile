COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app.exe -lws2_32 -Wall -O3
vm:
	$(COMPILER) vm.c -o build/vm.exe -lws2_32 -Wall