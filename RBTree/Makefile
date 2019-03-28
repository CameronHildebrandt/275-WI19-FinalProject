CXX=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=-o
PROGS=RBTree.cpp RBTree.h
OBJS=RBTree.o
EXE=RBTree

# Executable targets
all: $(EXE)

%.o: %.cpp
	$(CXX) $^ $(CFLAGS)

RBTree: RBTree.o
	$(CXX) $^ $(LFLAGS) $@

clean:
	@rm $(OBJS) $(EXE)
