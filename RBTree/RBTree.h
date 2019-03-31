// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#ifndef _RBTREE_
#define _RBTREE_

#include <assert.h>
#include <iostream>

// Node structure declairation
struct RBNode {
  int value;
  bool colour; // true = black ; false = red
  RBNode* parent;
  RBNode* left;
  RBNode* right;
};

// Tree structure declairation
class RBTree {
// You still need to add constructor//destructor


public:

  // Tree interaction functions
  RBNode* find(RBNode* root, int value);     //TODO
  RBNode* insert(RBNode* root, RBNode* node);   //BUG - dies to seg fault
  bool remove(int value);                       //TODO

private:
  // Functions for rotating about a node
  bool rRight(RBNode* node);
  bool rLeft(RBNode* node);

  // Helper functions
  RBNode* getParent(RBNode* node);
  RBNode* getGrandparent(RBNode* node);
  RBNode* getSibling(RBNode* node);
  RBNode* getUncle(RBNode* node);
  bool isLeaf(RBNode* node);

  void insertRecurse(RBNode* root, RBNode* node);

  // Functions for fixing tree upon node insertion
  void insertCase1(RBNode* node); // Fixes insert case 1
  // insertCase2 is trivial, so it doesn't need a function.
  void insertCase3(RBNode* node); // Fixes insert case 3
  void insertCase4(RBNode* node); // Rotates for insert case 4
  void insertCase4Fix(RBNode* node); // Fixes insert case 4 (after rotation)

  void fixTreeInsert(RBNode* node);

  // Functions for fixing tree upon node removal
  void removeCase1(RBNode* node); // Fixes remove case 1
  void removeCase2(RBNode* node); // Fixes remove case 2
  void removeCase3(RBNode* node); // Fixes remove case 3
  void removeCase4(RBNode* node); // Fixes remove case 4
  void removeCase5(RBNode* node); // Fixes remove case 5
  void removeCase6(RBNode* node); // Fixes remove case 6

  void replaceNode(RBNode* node, RBNode* child);

  void removeChild(RBNode* node); //BUG possibly errors here?
};

#endif
