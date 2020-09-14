COMPILER=gcc
OUTPUT=
ifeq ($(OS),Windows_NT)
	OUTPUT += build/app.exe
else
	OUTPUT += build/app
endif

module:
	$(COMPILER) *.c -o $(OUTPUT) -Wall -Ofast
run:
	$(OUTPUT) data/test.c 
test:
	valgrind --leak-check=yes -s build/app data/test.c
