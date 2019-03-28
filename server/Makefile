CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=

all: heapsort
heapsort: heapsort.o
	$(CC) heapsort.o -o heapsort $(LFLAGS)
binheap.o: heapsort.cpp heap.o
	$(CC) heapsort.cpp -o heapsort.o $(CFLAGS)
heap.o:
	$(CC) heap.h -o heap.o $(CFLAGS)

clean:
	rm heapsort heapsort.o heap.o
