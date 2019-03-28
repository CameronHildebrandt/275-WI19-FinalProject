// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include "RBTree.h"

// Tree structure implementation
RBNode* RBTree::getParent(RBNode* node) {
  return node->parent;
}


RBNode* RBTree::getGrandparent(RBNode* node) {
  RBNode* parent = getParent(node);

  return getParent(parent);
}


RBNode* RBTree::getSibling(RBNode* node) {
  RBNode* parent = getParent(node);

  // If there is no parent (root node), there are no siblings
  if(parent == NULL) {
    return NULL;
  }

  // If this is the left child, sibling is right
  else if(node == parent->left) {
    return parent->right;
  }

  // If this isn't the left child, sibling is left
  else {
    return parent->left;
  }
}


RBNode* RBTree::getUncle(RBNode* node) {
  RBNode* grandparent = getGrandparent(node);
  RBNode* parent = getParent(node);

  // There is no parent (this is the root node), there is no uncle
  if(parent == NULL) {
    return NULL;
  }

  // There is no grandparent (the parent is the root node), there is no uncle
  else if(grandparent == NULL) {
    return NULL;
  }

  // Find the parent's sibling
  return getSibling(parent);
}


RBNode* RBTree::find(RBNode* node) {
  return node;
}

bool RBTree::insert(int value) {
  return false;
}

void insertRecurse(RBNode* root, RBNode* n) {

}

bool RBTree::remove(int value) {
  return false;
}

bool RBTree::rRight(RBNode* node) {
  /* Inputs: pointer to the node to be rotated
   * Output: node rotation succeeded -> true; node rotation failed -> false
   * This function rotates the specified node right.
   */

  // Assign the new node to be the node that gets rotated up from the right
  RBNode* newNode = node->left;
  RBNode* parent = getParent(node);

  // Crash the program if the new node is a leaf, we cannot rotate a leaf
  if(newNode->right != NULL && newNode->left != NULL) {
    return false;
  }

  // Swap around the respective pointers
  node->left = newNode->right;
  newNode->right = node;
  node->parent = newNode;

  // If there is a left child, adopt them
  if (node->left != NULL) {
    node->left->parent = node;
  }

  // If the node is not the root, get new parents
  if (parent != NULL) {
    // If the old node is the left child, it's now the new node's left child
    if (node == parent->left) {
      parent->left = newNode;
    }
    // Same as above, but right
    else {
      parent->right = newNode;
    }
  }

  // fianlly, fix the new node's parent
  newNode->parent = parent;

  // if the function got here, the rotation was successful
  return true;
}


bool RBTree::rLeft(RBNode* node) {
  /* Inputs: pointer to the node to be rotated
   * Output: node rotation succeeded -> true; node rotation failed -> false
   * This function rotates the specified node left.
   */

  // Assign the new node to be the node that gets rotated up from the right
  RBNode* newNode = node->right;
  RBNode* parent = getParent(node);

  // Crash the program if the new node is a leaf, we cannot rotate a leaf
  if(newNode->right != NULL && newNode->left != NULL) {
    return false;
  }

  // Swap around the respective pointers
  node->right = newNode->left;
  newNode->left = node;
  node->parent = newNode;

  // If there is a right child, adopt them
  if (node->right != NULL) {
    node->right->parent = node;
  }

  // If the node is not the root, get new parents
  if (parent != NULL) {
    // If the old node is the left child, it's now the new node's left child
    if (node == parent->left) {
      parent->left = newNode;
    }
    // Same as above, but right
    else {
      parent->right = newNode;
    }
  }

  // fianlly, fix the new node's parent
  newNode->parent = parent;

  // if the function got here, the rotation was successful
  return true;
}


void RBTree::fixNode(RBNode* node) {

}

int main() {
  return 0;
}
