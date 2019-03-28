// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include "RBtree.h"

// Node structure
struct RBNode {
  int value;
  bool black = true;
  bool red = false;
  RBNode *grandparent;
  RBNode *parent;
  RBNode *uncle;
  RBNode *left;
  RBNode *right;
};

// Tree structure
class RBTree {
public:
  RBNode getParent(RBNode node);

  RBNode getGrandparent(RBNode node);

  RBNode getSibling(RBNode node);

  RBNode getUncle(RBNode node);

  RBNode find(RBNode node);

  bool insert(int value);

  bool delete(int value);

private:
  bool rRight(int value);

  bool rLeft(int value);

  void fixNode(RBNode node);
};
