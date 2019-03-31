CXX=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=-o
PROGS=RBTreeTest.cpp RBTree.cpp RBTree.h
OBJS=RBTree.o RBTreeTest.o
EXE=RBTree

# Executable targets
all: $(EXE)

%.o: %.cpp
	$(CXX) $^ $(CFLAGS)

RBTree: RBTree.o RBTreeTest.o
	$(CXX) $^ $(LFLAGS) $@

clean:
	@rm $(OBJS)

call:
	@rm $(OBJS) $(EXE)
