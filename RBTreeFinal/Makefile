# --------------------------------------------------------
# | Name: Cameron Hildebrandt
# | ID: 1584696
# | CMPUT 275, Winter 2019
# | Weekly Exercise #7 - AVL Trees
# --------------------------------------------------------

CXX=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=-o
PROGS=ex7test.cpp RBTreeAux.h RBTree.h
OBJS=ex7test.o
EXE=RBTree

# Executable targets
all: $(EXE)

%.o: %.cpp
	$(CXX) $^ $(CFLAGS)

RBTree: ex7test.o
	$(CXX) $^ $(LFLAGS) $@

clean:
	@rm $(OBJS)

clall:
	@rm $(OBJS) $(EXE)
