// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include <iostream>
#include <assert.h>

// Node structure declairation
struct RBNode {
  int value;
  bool black;
  bool red;
  RBNode* parent;
  RBNode* left;
  RBNode* right;
};

// Tree structure declairation
class RBTree {
public:
  RBNode* getParent(RBNode* node);

  RBNode* getGrandparent(RBNode* node);

  RBNode* getSibling(RBNode* node);

  RBNode* getUncle(RBNode* node);

  RBNode* find(RBNode* node);

  bool insert(int value);

  void insertRecurse(RBNode* node);

  bool remove(int value);

private:
  bool rRight(RBNode* node);

  bool rLeft(RBNode* node);

  void fixNode(RBNode* node);
};
