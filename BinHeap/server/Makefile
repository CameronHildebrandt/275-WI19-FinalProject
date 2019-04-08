CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=

all: server
heapsort: server.o
	$(CC) server.o -o heapsort $(LFLAGS)
server.o: server.cpp heap.o serialport.o
	$(CC) server.cpp -o server.o $(CFLAGS)
heap.o:
	$(CC) heap.h -o heap.o $(CFLAGS)
serialport.o:
	$(CC) serialport.h -o serialport.o $(CFLAGS)

clean:
	rm server server.o heap.o serialport.o
