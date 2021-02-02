CC      := gcc
LD      := ld
BIN     := bin
SRCS    := $(wildcard *.c)
EXE     := $(BIN)/chipcode
CFLAGS  := -Wall -std=c99 -Ofast -s
LIBS    := 
ifeq ($(OS),Windows_NT)
	LIBS := $(LIBS) -lws2_32
endif

.PHONY: clean install

all: 
	./build.sh
run:
	$(EXE) data/test.c
clean:
	rm -rf bin/*
