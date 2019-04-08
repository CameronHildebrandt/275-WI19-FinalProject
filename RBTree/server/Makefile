# --------------------------------------------------------
# | Name: Cameron Hildebrandt, 1584696
# | Name: Ramana Vasanthan, 1458497
# | CMPUT 275, Winter 2019
# | Final Project - Data Structure Visualizer
# --------------------------------------------------------

CXX=g++
CFLAGS=-c -Wall -O2 -std=c++11
LFLAGS=-o
PROGS=RBTreeInterface.cpp RBTreeAux.h RBTree.h
OBJS=RBTreeInterface.o
EXE=RBTree

# Executable targets
all: $(EXE)

%.o: %.cpp
	$(CXX) $^ $(CFLAGS)

RBTree: RBTreeInterface.o
	$(CXX) $^ $(LFLAGS) $@

clean:
	@rm $(OBJS)

clall:
	@rm $(OBJS) $(EXE)
