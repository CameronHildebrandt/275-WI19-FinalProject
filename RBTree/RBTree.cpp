// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include "RBTree.h"

// Initialize the required variables
// RBNode node;
// RBNode root;
// RBTree tree;

// Helper functions
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

bool RBTree::isLeaf(RBNode* node) {
  if(node->right != NULL && node->left != NULL) {
    return false;
  }
  return true;
}

void RBTree::insertRecurse(RBNode* root, RBNode* node) {
  // Find a leaf node where the value should be inserted

  // the node being inserted is less than the current node
  if(root != NULL && node->value < root->value) {

    if (root->left != NULL) {  // the left child isn't a leaf
      insertRecurse(root->left, node);
      return;
    }
    else {  // the left child is a leaf, so this is where to insert
      root->left = node;
    }
  }

  // the node being inserted is greater than the current node
  else if(root != NULL) {
    if (root->right != NULL) {  // the right child isn't a leaf
      insertRecurse(root->right, node);
      return;
    }
    else {  // the right child is a leaf, so this is where to insert
      root->right = node;
    }
  }

  // Insert the node
  node->parent = root;
  node->left = NULL; // this is a leaf
  node->right = NULL;
  node->colour = false; // false = red
}


// Tree interaction functions
RBNode* RBTree::find(RBNode* root, int value) {
  /* Inputs: value - The node to search for
   *         root - the root of the tree
   * Output: node || NULL - The node found, NULL if the node is not found.
   * Walk down the tree, going left or right when appropriate. Eventually, you
   * will either find the value or fall off of the tree.
   */

  RBNode* searchNode = root;
  // RBNode* parent = root;

	while(value != searchNode->value && searchNode != NULL) {
		// parent = searchNode->parent;

		if(value < searchNode->value) {
			searchNode = searchNode->left;
		}
		else {
			searchNode = searchNode->right;
		}
	}

	if(searchNode == NULL) {
		// key not found
		return NULL;
	}
	else {
		// key found
		return searchNode;
	}
}

RBNode* RBTree::insert(RBNode* root, RBNode* node) {
  /* Inputs: root - The root of the tree
   *         node - The node to be inserted
   * Output: root - The root of the tree (after rotations)
   * This fucntion puts a given node into the RBTree.
   */

  // Require that the node is red (all new nodes are red)
  node->colour = false; //false == red

  // Insert the node
  insertRecurse(root, node);

  // Satisfy the RB properties
  fixTreeInsert(node);

  // Find the new root of the tree
  root = node;
  while (getParent(root) != NULL) {
    root = getParent(root);
  }

  return root;
}

bool RBTree::remove(int value) {
  return false;
}


// Rotation funtion implementations
bool RBTree::rRight(RBNode* node) {
  /* Inputs: pointer to the node to be rotated
   * Output: node rotation succeeded -> true; node rotation failed -> false
   * This function rotates the specified node right.
   */

  // Assign the new node to be the node that gets rotated up from the right
  RBNode* newNode = node->left;
  RBNode* parent = getParent(node);

  // Terminate function if the new node is a leaf, we cannot rotate a leaf
  if(isLeaf(newNode)) {
    std::cout << "Error: Attempted to rotate about a leaf\n";
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

  // Terminate function if the new node is a leaf, we cannot rotate a leaf
  if(isLeaf(newNode)) {
    std::cout << "Error: Attempted to rotate about a leaf\n";
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


// Functions for fixing tree upon node insertion
void RBTree::insertCase1(RBNode* node) {
  /* This fucntion ensures that if the node is the root, it is black
   */
  if (getParent(node) == NULL) {
    node->colour = true; //true == black
  }
}

void RBTree::insertCase3(RBNode* node) {
  /* This function ensures that red nodes do not have red children
   */

  // Reassign the respective colours
  getParent(node)->colour = true; //true == black
  getUncle(node)->colour = true; //true == black
  getGrandparent(node)->colour = false; //false == red

  // Changing the grandparent node could have caused violations, so recurse
  // to fix
  fixTreeInsert(getGrandparent(node));
}

void RBTree::insertCase4(RBNode* node) {
  /* This function conducts rotations to reduce the depth of the tree
   */

  // Declare the relations
  RBNode* parent = getParent(node);
  RBNode* grandparent = getGrandparent(node);

  // The tree needs to be rotated left if it is a right child
  if (node == parent->right && parent == grandparent->left) {
    rLeft(parent);
    node = node->left;
  }

  // The tree needs to be rotated right if it is a left child
  else if (node == parent->left && parent == grandparent->right) {
    rRight(parent);
    node = node->right;
  }

  // This likely violates tree properties, so fix the problems the
  // rotations casued.
  insertCase4Fix(node);
}

void RBTree::insertCase4Fix(RBNode* node) {
  /* This function fixes the properties now violated by rotating the node's
   * grandparent and changing the colour of the parent and grandparent
   */

  // Declare the relations
  RBNode* parent = getParent(node);
  RBNode* grandparent = getGrandparent(node);

  // If the node is a left child, you have to rotate right
  if (node == parent->left) {
    rRight(grandparent);
  }
  else {
    rLeft(grandparent);
  }

  // Fix the colours
  parent->colour = true; //true == black
  grandparent->colour = false; //false == red
}

void RBTree::fixTreeInsert(RBNode* node) {
  /* Inputs: The node to verify the properties of
   * Output: None
   * This function validates and fixes the properties of the RBTree.
   */

  // Case 1 - If the node is the root, ensure it is black
  if (getParent(node) == NULL) {
    insertCase1(node);
  }
  // Case 2 - If the node is not the root, ensure the parent is black
  else if (getParent(node)->colour == true) {
    return; // Since the inserted node is red, the tree properties are
            // satisfied.
  }
  // Case 3 - The parent and uncle are not black, so simply change colours
  else if (getUncle(node) != NULL && getUncle(node)->colour == false) {
    insertCase3(node);
  }
  // Case 4 - The parent is not black, but the uncle is, so start rotating
  else {
    insertCase4(node);
  }
}


// Functions for fixing tree upon node removal
void RBTree::removeCase1(RBNode* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  if (node->parent != NULL) {
    removeCase2(node);
  }
}

void RBTree::removeCase2(RBNode* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode* sibling = getSibling(node);

  if(sibling->colour == false) { //false == red
    node->parent->colour = false; //false == red
    sibling->colour = true; //true == black

    if (node == node->parent->left) {
      rLeft(node->parent);
    }
    else {
      rRight(node->parent);
    }
  }
  removeCase3(node);
}

void RBTree::removeCase3(RBNode* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode* sibling = getSibling(node);

  if((node->parent->colour == true)    && //true == black
     (sibling->colour == true)         &&
     (sibling->left->colour == true)   &&
     (sibling->right->colour == true)) {

    sibling->colour = false; //false == red

    removeCase1(node->parent);
  }
  else {
    removeCase4(node);
  }
}

void RBTree::removeCase4(RBNode* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode* sibling = getSibling(node);

  if((node->parent->colour == false)   && //false == red
     (sibling->colour == true)         && //true == black
     (sibling->left->colour == true)   &&
     (sibling->right->colour == true)) {
    sibling->colour = false;
    node->parent->colour = true;
  }
  else {
    removeCase5(node);
  }
}

void RBTree::removeCase5(RBNode* node) {
/* Inputs: node - The node with violated RBTree properties to fix
 * Output: None
 *
 */

  // Initialize the sibling
  RBNode* sibling = getSibling(node);

  // This if statement is trivial,since no red parent can have a red child
  if(sibling->colour == true) { //true == black
    // The following statements force the red to be on the left of the left
    // of the parent,or right of the right, so case six will rotate correctly
    if((node == node->parent->left)      &&
       (sibling->right->colour == true)  &&
       (sibling->left->colour == false)) { // this test is trivial due to cases 2-4
      sibling->colour = false;
      sibling->left->colour = true;
      rRight(sibling);
    }
    else if((node == node->parent->right)      &&
            (sibling->left->colour == true)    &&
            (sibling->right->colour == false)) { // this test is trivial due to cases 2-4
      sibling->colour = false;
      sibling->right->colour = true;
      rLeft(sibling);
    }
  }
  removeCase6(node);
}

void RBTree::removeCase6(RBNode* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode* sibling = getSibling(node);

  sibling->colour = node->parent->colour;
  node->parent->colour = true; //true == black

  if(node == node->parent->left) {
    sibling->right->colour = true; //true == black
    rLeft(node->parent);
  }
  else {
    sibling->left->colour = true; //true == black
    rRight(node->parent);
  }
}

void RBTree::replaceNode(RBNode* node, RBNode* child) {
  /* Inputs: node - The node to be replaced
   *         child - The node that is replacing "node"
   * Output: None
   * This fucntion replaces a node
   */

  // Set the new child to have the same parent
  child->parent = node->parent;

  // Replace the parent's pointer to point to the new child
  if (node == node->parent->left) {
    node->parent->left = child;
  }
  else {
    node->parent->right = child;
  }
}

void RBTree::removeChild(RBNode* node) {
  /* Precondition: node has at most one non-leaf child
   * Inputs: node - the node to remove the child from
   * Output: None
   * This function strps off the child of a node.
   */

   if(!isLeaf(node->right) && !isLeaf(node->left)) {
     std::cout << "Error: Attempted to remove a node without a child " <<
                  "that is a leaf node.\n";
      return;
   }

  // Select the child that is not a leaf
  RBNode* child = isLeaf(node->right) ? node->left : node->right;

  // Replace the current node
  replaceNode(node, child);

  // If the node is black and the child is red, make the child black
  if (node->colour == true) { //true == black
    if (child->colour == false) { //false == red
      child->colour = true; //true == black
    }
  else
    removeCase1(child);
  }

  // Remove node from memory
  free(node);
}
