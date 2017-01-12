CC=gcc
CFLAGS=-c -Wall -g

all: othello

othello.o: othello.c othello.h validation.h helper.h astar.h
	$(CC) $(CFLAGS) othello.c -o othello.o

othello: othello.o
	$(CC) -lcurses othello.o -o othello

clean: 
	rm *.o othello
