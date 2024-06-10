CC = g++
CFLAGS = -I. -Wall -std=c++11
LDFLAGS = -lncurses

all: main

main: sound_gen.cpp main.cpp
	$(CC) $(CFLAGS) sound_gen.cpp main.cpp $(LDFLAGS) -o main


