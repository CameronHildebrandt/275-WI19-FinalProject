// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Data Structure Visualizer
// --------------------------------------------------------

#ifndef _RBTREE_H_
#define _RBTREE_H_

// Include the required libraries
#include <cassert>
#include <iostream>
#include <cstdlib>
#include "RBTreeAux.h"
#include <string>

using namespace std;

// Define the colours that nodes may be
enum Colour {RED, BLACK};


template <typename K, typename T>
class RBTree {
	/* A red-black tree. This is a type of self-balancing binary tree that
	 * maintains a depth of O(log n) through using rotations similar to an AVL
	 * tree. All operations (insertions, deletions, and searching) on this data
	 * structure take O(log n) time.
	 * Assumptions: K can be compared using < and >
	 */
public:
	// Constructor
	RBTree();

	// Destructor
	~RBTree();

	// Insert an item (or update the item at a given key)
	void update(const K& key, const T& item);

	// Delete a key
	void remove(const K& key);

	// Determines if a key is in the tree
	bool hasKey(const K& key) const;

	// Operator overload so this data structure may be used similar to an array
	T& operator[](const K& key);

	// Return the size of the tree
	unsigned int size() const;

	// Return an iterator to the first item (ordered by key)
	RBIterator<K,T> begin() const;

	// returns an iterator signalling the end iterator
	RBIterator<K,T> end() const;

	// Print the tree in an order defined in the README
	void treePrint();

private:
	// Stoe important tree information
	RBNode<K,T> *root;
	unsigned int RBSize;

	// Return a pointer to the node if it is found. If the node does not exist,
	// a pointer to it's parent would be. Reurns "NULL" if the tree is empty.
	RBNode<K,T>* findNode(const K& key) const;

	// The rucursive function for the treePrint function
	void treeRecurse(RBNode<K,T>* node);

	// This is expecting either a leaf node or a node with only one child.
	// This function simply reassigns pointers to remove the node from the tree
	void pluckNode(RBNode<K,T>* node);

	// Left and right rotations. Rotate about a node, returning the new root of
	// the subtree. Expects to be passed a node that has left and right children.
	RBNode<K,T>* rotateLeft(RBNode<K,T>* node);
	RBNode<K,T>* rotateRight(RBNode<K,T>* node);

	// Helper functions
	RBNode<K,T>* getParent(RBNode<K,T>* node);

	RBNode<K,T>* getGrandparent(RBNode<K,T>* node);

	RBNode<K,T>* getSibling(RBNode<K,T>* node);

	RBNode<K,T>* getUncle(RBNode<K,T>* node);

	bool isLeaf(RBNode<K,T>* node);


	// Functions for fixing tree upon node insertion
	void insertCase1(RBNode<K,T>* node);

	void insertCase3(RBNode<K,T>* node);

	void insertCase4(RBNode<K,T>* node);

	void insertCase4Fix(RBNode<K,T>* node);

	void fixTreeInsert(RBNode<K,T>* node);


	// Functions for fixing tree upon node deletion
	void removeCase1(RBNode<K,T>* node);

	void removeCase2(RBNode<K,T>* node);

	void removeCase3(RBNode<K,T>* node);

	void removeCase4(RBNode<K,T>* node);

	void removeCase5(RBNode<K,T>* node);

	void removeCase6(RBNode<K,T>* node);

	void replaceNode(RBNode<K,T>* node, RBNode<K,T>* child);

	void removeChild(RBNode<K,T>* node);
};


// ***** Public Methods ***** //


template <typename K, typename T>
RBTree<K,T>::RBTree() {
	/* Constructor - Initialize an empty tree with a NULL root node
	 */
	this->root = NULL;
	this->RBSize = 0;
}

template <typename K, typename T>
RBTree<K,T>::~RBTree() {
	/* Destructor - Empty the tree
	 */

	 // If this condition fails, then the tree is already empty
	if (this->root != NULL) {

		// Calls the recursive destructor in RBNode
		delete this->root;

		this->root = NULL;

		this->RBSize = 0;
	}
}

template <typename K, typename T>
void RBTree<K,T>::update(const K& key, const T& item) {
	/* Insert an element
	 */

	// Find the position that an element should be inserted
	RBNode<K,T>* node = findNode(key);

	// If there was no node in the tree with this key, create one
	if (node == NULL || node->key != key) {
		RBNode<K,T> *newNode = new RBNode<K,T>(key, item, NULL, NULL, node, RED);
		assert(newNode != NULL);

		// Change the left or right pointer of the parent node, whichever is
		// appropriate to preserve the RBTree properties
		if (node == NULL) {
			// The tree was empty, so this is the new root
			root = newNode;
		}
		else {
			// The tree was not empty, so put it as the appropriate child of "node"
			if (key < node->key) {
				node->left = newNode;
			}
			else {
				node->right = newNode;
			}
		}
		++RBSize;

		// Go through the tree insert cases to ensure that the RBTree properties
		// remain satisfied
		fixTreeInsert(newNode);
	}

	else {
		// If the key already existed, simply update the item
		node->item = item;
	}
}

template <typename K, typename T>
void RBTree<K,T>::remove(const K& key) {
	/* Remove an element
	 */

	// Find the node to be deleted. If it does not exist.. don't delete it
	RBNode<K,T>* node = findNode(key);

	// If the key is not in the tree, terminate the removal
	if(node == NULL && !(node->key < key || key < node->key)) {
		return;
	}

	// Find the maximum-key node in the left subtree of the node to remove
	RBNode<K,T> *tmp = node->left, *pluck = node;
	while (tmp != NULL) {
		pluck = tmp;
		tmp = tmp->right;
	}

	// Copy contents from node we are deleting
	node->key = pluck->key;
	node->item = pluck->item;

	// Pluck the node. The pluckNode function itself automatically checks and
	// fixes the RBTree properties.
	pluckNode(pluck);

}

template <typename K, typename T>
bool RBTree<K,T>::hasKey(const K& key) const {
	/* This function returns true if the key is in the tree
	 */

	// Use the previously implemented findNode to find a given key. If the found
	// node is either not NULL and also not the parent node (which the
	// "findNode") may also return, then the node is in the tree!
	RBNode<K,T> *node = findNode(key);
	return node != NULL && !(node->key != key);
}

template <typename K, typename T>
T& RBTree<K,T>::operator[](const K& key) {
	/* This function inserts a given key into a tree and returns the item of the
	 * newly inserted key
	 */

	// If the node isn't already in the tree, insert it
	if (!hasKey(key)) {
		update(key, T());
	}

	return findNode(key)->item;
}

template <typename K, typename T>
unsigned int RBTree<K,T>::size() const {
	/* Return the size of the tree
	 */

	return this->RBSize;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::findNode(const K& key) const {
	/* This function takes advantage of the properties of a binary tree and
	 * traverses down the tree to find a node. If the search falls off of the
	 * tree (it finds "NULL") then it returns either the parent (where the
	 * key would be inserted) or NULL if the tree is empty. If the search doesn't
	 * fall off of the tree, return the node.
	 */

	// Initialize the nodes that will be used for searching
	RBNode<K,T> *node = this->root, *parent = NULL;

	// Traverse down the tree, going left and right as appropriate,
	// until the key is found or we fall off of the leaf node
	while (node != NULL && node->key != key) {
		parent = node;
		if (key < node->key) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	if (node == NULL) {
		// Key not found
		// This is only satisfied if parent == NULL, which means the tree is empty
		return parent;
	}
	else {
		// Key found
		return node;
	}
}

template <typename K, typename T>
void RBTree<K,T>::treeRecurse(RBNode<K,T>* node) {
	/* This function recurses down the left side of the tree, printing out the
	 * keys (and colours) of each node as it recurses. Expects to be initially
	 * passed the root node, If it isn't, it will just print out the subtree
	 * starting at the given node.
	 */

	// Print out the node's key and colour
	cout << node->key << ',' << node->colour << ' ';

	// Recurse down the left of the tree first, then work our way right.
	if(node->left != NULL) {
		treeRecurse(node->left);
	}
	cout << "R ";

	// Recurse down the right
	if(node->right != NULL) {
		treeRecurse(node->right);
	}
	cout << "U ";
}

template <typename K, typename T>
void RBTree<K,T>::treePrint() {

	RBNode<K,T> *root = this->root;
	// Traverse down the tree, going left and right as appropriate,
	// until a leaf is found, print up recursively.
	// Suppose the tree was:   20
	//											 5    31
	// 										  1 7  25 33
	// This function would print it as:
	// 20,1 5,1 1,0 R U R 7,0 R U U R 31,1 25,0 R U R 33,0 R U U U

	// If the tree isn't empty, pass the root and begin recursively
	// printing nodes
	if(root != NULL) {
		treeRecurse(root);
	}
	cout << endl;
}

template <typename K, typename T>
RBIterator<K,T> RBTree<K,T>::begin() const {
	// An RBIterator is just a wrapper for a pointer to a node
	return RBIterator<K,T>(this->root);
}

template <typename K, typename T>
RBIterator<K,T> RBTree<K,T>::end() const {
	// The NULL pointer represents the end iterator (whether thats an empty root
	// or falling off of a leaf)
	return RBIterator<K,T>(NULL);
}


// ***** Private Methods ***** //


template <typename K, typename T>
void RBTree<K,T>::pluckNode(RBNode<K,T>* node) {
	/* This is expecting either a leaf node or a node with only one child.
	 * This function simply reassigns pointers to remove the node from the tree
	 */

	// First find the only child (if any) of "node"
	RBNode<K,T> *child;
	if(node->left) {
		child = node->left;
		// Make sure the node does not have two children
		assert(child->right == NULL);
	}
	else {
		// Might still be NULL, meaning we are plucking a leaf node
		child = node->right;
	}


	// Adjust the appropriate child pointer of the node's parent
	if(node->parent == NULL) {
		// In this case, we are deleting the root node
		// So set the new root to the child
		this->root = child;
	}
	else if(node->parent->left == node) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}

	// If we are not deleting a leaf, the child also gets a new parent
	if(child) {
		child->parent = node->parent;
	}

	// Fix the RB properties
	// If the node is black and the child is red, paint the child black
  if (node->colour == BLACK && node->parent != NULL) {
    if (child->colour == RED) {
      child->colour = BLACK;
    }
	  else {
	    removeCase1(child);
		}
  }

	// Ensures ~RBNode() does not recursively delete other parts of the tree
	node->left = node->right = NULL;

	// The node is now gone!
	delete node;
	--RBSize;

}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::rotateRight(RBNode<K,T>* node) {
	/* A right rotation. This function preforms a right rotation on the node
	 * passed to it. This is fundamental to maintaining the RBTree
	 * properties. This function reassigns pointers to move around nodes of
	 * the tree.
	 */

	// Find the left child of the given node.
	RBNode<K,T> *lchild = node->left;
	assert(left != NULL);

	// Reassign the parents
	if (node->parent) {
		if (node == node->parent->left) {
			node->parent->left = lchild;
		}
		else {
			node->parent->right = lchild;
		}
	}
	else {
		root = lchild;
	}

	// Reassign the children
	lchild->parent = node->parent;
	node->parent = lchild;
	if (lchild->right) {
		lchild->right->parent = node;
	}

	node->left = lchild->right;
	lchild->right = node;

	// Return the "lchild", as this is the new root of this subtree.
	return lchild;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::rotateLeft(RBNode<K,T>* node) {
	/* A left rotation. This function preforms a left rotation on the node
	 * passed to it. This is fundamental to maintaining the RBTree
	 * properties. This function reassigns pointers to move around nodes of
	 * the tree.
	 */

	// Find the right child of the given node.
	RBNode<K,T> *rchild = node->right;
	assert(right != NULL);

	// To track all of these changes, it is best to
	// draw your own picture with all relevant pointers
	// to see these operations change them properly.

	// Reassign the parents
	if (node->parent) {
		if (node == node->parent->left) {
			node->parent->left = rchild;
		}
		else {
			node->parent->right = rchild;
		}
	}
	else {
		root = rchild;
	}

	// Reassign the children
	rchild->parent = node->parent;
	node->parent = rchild;
	if (rchild->left) {
		rchild->left->parent = node;
	}

	node->right = rchild->left;
	rchild->left = node;

	// Return the "rchild", as this is the new root of this subtree.
	return rchild;
}


// ***** Helper Functions ***** //


template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getParent(RBNode<K,T>* node) {
	/* Returns the parent of a node.
	 */
  return node->parent;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getGrandparent(RBNode<K,T>* node) {
	/* Returns the grandparent of a node.
	 */
  RBNode<K,T>* parent = getParent(node);

	// If the parent does not have a parent, return NULL, as there is no
	// grandparent.
	if(parent == NULL) {
		return NULL;
	}

  return getParent(parent);
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getSibling(RBNode<K,T>* node) {
	/* Returns the sibling of a node (if it exists).
	 */
  RBNode<K,T>* parent = getParent(node);

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

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getUncle(RBNode<K,T>* node) {
	/* Returns the uncle of a node (if it exists).
	 */

	// Assign variables to the necessary nodes
  RBNode<K,T>* grandparent = getGrandparent(node);
  RBNode<K,T>* parent = getParent(node);

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

template <typename K, typename T>
bool RBTree<K,T>::isLeaf(RBNode<K,T>* node) {
	/* Determines if a given node is a leaf
	 */
  if(node->right == NULL && node->left == NULL) {
    return true;
  }
  return false;
}


// Tree Insertion //


template <typename K, typename T>
void RBTree<K,T>::insertCase1(RBNode<K,T>* node) {
  /* This fucntion ensures that if the node is the root, it is black
   */
  if (getParent(node) == NULL) {
    node->colour = BLACK;
  }
}

template <typename K, typename T>
void RBTree<K,T>::insertCase3(RBNode<K,T>* node) {
  // This function ensures that red nodes do not have red children

  // Reassign the respective colours
  getParent(node)->colour = BLACK;
  getUncle(node)->colour = BLACK;
  getGrandparent(node)->colour = RED;

  // Changing the grandparent node could have caused violations, so recurse
  // to fix
  fixTreeInsert(getGrandparent(node));
}

template <typename K, typename T>
void RBTree<K,T>::insertCase4(RBNode<K,T>* node) {
  /* This function conducts rotations to reduce the depth of the tree
   */

  // Declare the relations
  RBNode<K,T>* parent = getParent(node);
  RBNode<K,T>* grandparent = getGrandparent(node);

  // The tree needs to be rotated left if it is a right child
  if (node == parent->right && parent == grandparent->left) {
    rotateLeft(parent);
    node = node->left;
  }

  // The tree needs to be rotated right if it is a left child
  else if (node == parent->left && parent == grandparent->right) {
    rotateRight(parent);
    node = node->right;
  }

  // This likely violates tree properties, so fix the problems the
  // rotations casued.
  insertCase4Fix(node);
}

template <typename K, typename T>
void RBTree<K,T>::insertCase4Fix(RBNode<K,T>* node) {
  /* This function fixes the properties now violated by rotating the node's
   * grandparent and changing the colour of the parent and grandparent
   */

  // Declare the relations
  RBNode<K,T>* parent = getParent(node);
  RBNode<K,T>* grandparent = getGrandparent(node);

  // If the node is a left child, you have to rotate right
  if (node == parent->left) {
    rotateRight(grandparent);
  }
  else {
    rotateLeft(grandparent);
  }

  // Fix the colours
  parent->colour = BLACK;
  grandparent->colour = RED;
}

template <typename K, typename T>
void RBTree<K,T>::fixTreeInsert(RBNode<K,T>* node) {
  /* Inputs: The node to verify the properties of
   * Output: None
   * This function validates and fixes the properties of the RBTree.
   */

  // Case 1 - If the node is the root, ensure it is black
  if (getParent(node) == NULL) {
    insertCase1(node);
  }
  // Case 2 - If the node is not the root, ensure the parent is black
  else if (getParent(node)->colour == BLACK) {
    return; // Since the inserted node is red, the tree properties are
            // satisfied, kill before anything is broken.
  }
  // Case 3 - The parent and uncle are not black, so simply change colours
  else if (getUncle(node) != NULL && getUncle(node)->colour == RED) {
    insertCase3(node);
  }
  // Case 4 - Every other case
  else {
    insertCase4(node);
  }
}


// Tree Removal //


template <typename K, typename T>
void RBTree<K,T>::removeCase1(RBNode<K,T>* node) {
  /* This function ensures that a nodes parents exist. If they don't, no other
	 * removal cases can be violated.
   */

  if (node->parent != NULL) {
    removeCase2(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase2(RBNode<K,T>* node) {
  /* This funtion ensures that a node cannot be red and have red children
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

	// Check the case
  if(sibling->colour == RED) {
    node->parent->colour = RED;
    sibling->colour = BLACK;

    if (node == node->parent->left) {
      rotateLeft(node->parent);
    }
    else {
      rotateRight(node->parent);
    }
  }

	// The case is either fixed or was not violated to begin with, contnue.
  removeCase3(node);
}

template <typename K, typename T>
void RBTree<K,T>::removeCase3(RBNode<K,T>* node) {
  /* This function ensures that there is not a large grouping of black nodes
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  if((node->parent->colour == BLACK)    &&
     (sibling->colour == BLACK)         &&
     (sibling->left->colour == BLACK)   &&
     (sibling->right->colour == BLACK)) {

    sibling->colour = RED;

		// May have violated more of the tree, fix the cases for the parent node
		// before continuing.
    removeCase1(node->parent);
  }
  else {
		// The case was not violated, continue.
    removeCase4(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase4(RBNode<K,T>* node) {
  /* This function also ensures that there is not a large grouping of
	 * black nodes, just shifted down a little from the previous case.
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  if((node->parent->colour == RED)   		&&
     (sibling->colour == BLACK)         &&
     (sibling->left->colour == BLACK)   &&
     (sibling->right->colour == BLACK)) {

    sibling->colour = RED;
    node->parent->colour = BLACK;
  }
  else {
		// The case was not violated, continue.
    removeCase5(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase5(RBNode<K,T>* node) {
	/* This case ensures that the right black depths are satisfied. If they are
	 * not, then begin preformign rotations.
	 */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  // This if statement is trivial, since no red parent can have a red child
  if(sibling->colour == BLACK) {

    // The following statements force the red to be on the left of the left
    // of the parent, or right of the right, so case six will rotate correctly
    if((node == node->parent->left)       &&
       (sibling->right->colour == BLACK)  &&
       (sibling->left->colour == RED)) {
      sibling->colour = RED;
      sibling->left->colour = BLACK;
      rotateRight(sibling);
    }
    else if((node == node->parent->right)       &&
            (sibling->left->colour == BLACK)    &&
            (sibling->right->colour == RED)) {
      sibling->colour = RED;
      sibling->right->colour = BLACK;
      rotateLeft(sibling);
    }
  }

	// The case may or may not have completely fixed the tree, continue.
  removeCase6(node);
}

template <typename K, typename T>
void RBTree<K,T>::removeCase6(RBNode<K,T>* node) {
  /* This case ensures that the rotations of the previous case did not
	 * violate the RBTree properties, and preforms the final rotations if
	 * necessary.
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  sibling->colour = node->parent->colour;
  node->parent->colour = BLACK;

  if(node == node->parent->left) {
    sibling->right->colour = BLACK;
    rotateLeft(node->parent);
  }
  else {
    sibling->left->colour = BLACK;
    rotateRight(node->parent);
  }

	// There are no more ways to violate the RBTree properties.
}

template <typename K, typename T>
void RBTree<K,T>::replaceNode(RBNode<K,T>* node, RBNode<K,T>* child) {
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

template <typename K, typename T>
void RBTree<K,T>::removeChild(RBNode<K,T>* node) {
  /* Precondition: node has at most one non-leaf child
   * Inputs: node - the node to remove the child from
   * Output: None
   * This function strps off the child of a node.
   */

	if(!isLeaf(node->right) && !isLeaf(node->left)) {
		cout << "Error: Attempted to remove a node without a child " <<
		        "that is a leaf node.\n";
		return;
	}

  // Select the child that is a leaf
  RBNode<K,T>* child = isLeaf(node->right) ? node->left : node->right;

  // Replace the current node
  replaceNode(node, child);

  // If the node is black and the child is red, paint the child black
  if (node->colour == BLACK) {
    if (child->colour == RED) {
      child->colour = BLACK;
    }
  else
    removeCase1(child);
  }

	// ensures ~RBNode() does not recursively delete other parts of the tree
	node->left = node->right = NULL;

	// Remove the node from memory
	delete node;
	--RBSize;
}

#endif
