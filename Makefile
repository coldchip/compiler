COMPILER=gcc

module:
	$(COMPILER) *.c -o build/app.exe -lws2_32 -Wall -O0 -g
vm:
	$(COMPILER) vm.c -o build/vm.exe -lws2_32 -Wall