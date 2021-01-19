CC      := gcc
LD      := ld
BIN     := bin
SRCS    := $(wildcard *.c)
EXE     := $(BIN)/chipcode
CFLAGS  := -Wall -std=c99 -Ofast -s
LIBS    := -lpthread
ifeq ($(OS),Windows_NT)
	LIBS := $(LIBS) -lws2_32
endif

.PHONY: clean install

all: $(EXE)

$(EXE): $(SRCS) | $(BIN)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $@
	rm -rf $(BIN)/tmp_*
run:
	$(EXE) data/test.c
clean:
	rm -rf bin/*

upload:
	scp -r /home/ryan/chiphttp/* ryan@192.168.0.100:/home/ryan/chipdrive