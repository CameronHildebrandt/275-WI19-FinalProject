// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
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

// ***** IMPLEMENTATION *****

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
	/* Insert an element
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

	// Pluck the node
	pluckNode(pluck);

	// Ensure the RBTree properties remain staisfied
	//removeChild(pluck->parent);

}

template <typename K, typename T>
bool RBTree<K,T>::hasKey(const K& key) const {

	// "find" the node, and then check it really has the key
	RBNode<K,T> *node = findNode(key);
	return node != NULL && !(node->key != key);
}

template <typename K, typename T>
T& RBTree<K,T>::operator[](const K& key) {

	// "find" the node, if not found then create an entry
	// using the default constructor for the item type
	if (!hasKey(key)) {
		update(key, T());
	}

	return findNode(key)->item;
}

template <typename K, typename T>
unsigned int RBTree<K,T>::size() const {
	return this->RBSize;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::findNode(const K& key) const {
	RBNode<K,T> *node = this->root, *parent = NULL;
	// traverse down the tree, going left and right as appropriate,
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
		// key not found
		// here, parent == NULL if and only if the tree is empty
		return parent;
	}
	else {
		// key found
		return node;
	}
}


template <typename K, typename T>
void RBTree<K,T>::treeRecurse(RBNode<K,T>* node) {
	// Output the value of this node before recursing

	cout << node->key << ',' << node->colour << ' ';
	// keep+=node->key+48;
	// keep +=" ";
	//
	// keep+= node->colour+48;
	// keep+=" ";
	// cout<<keep<<endl;


	// Recurse down the left of the tree first, then work our way right.
	if(node->left != NULL) {
		treeRecurse(node->left);
	}
	// keep+="R ";
	cout << "R ";
	if(node->right != NULL) {
		treeRecurse(node->right);
	}
	// keep +="U ";
	cout << "U ";
}

template <typename K, typename T>
void RBTree<K,T>::treePrint() {
	// keep.clear();
	RBNode<K,T> *root = this->root;
	// Traverse down the tree, going left and right as appropriate,
	// until a leaf is found, print up recursively.
	// Suppose the tree was:   20
	//											 5    31
	// 										  1 7  25 33
	// This function would print it as:
	// 20 5 1 R U R 7 U U 31 25 33

	if(root != NULL) {
		treeRecurse(root);
	}
	cout << endl;
	//return keep;

	// while (node != NULL && node->key != key) {
	// 	parent = node;
	// 	if (key < node->key) {
	// 		node = node->left;
	// 	}
	// 	else {
	// 		node = node->right;
	// 	}
	// }
	// if (node == NULL) {
	// 	// key not found
	// 	// here, parent == NULL if and only if the tree is empty
	// 	return parent;
	// }
	// else {
	// 	// key found
	// 	return node;
	// }
}

// an RBIterator is just a wrapper for a pointer to a node
template <typename K, typename T>
RBIterator<K,T> RBTree<K,T>::begin() const {
	return RBIterator<K,T>(this->root);
}

// the NULL pointer represents the end iterator
template <typename K, typename T>
RBIterator<K,T> RBTree<K,T>::end() const {
	return RBIterator<K,T>(NULL);
}


// ***** NOW THE PRIVATE METHODS *****

template <typename K, typename T>
void RBTree<K,T>::pluckNode(RBNode<K,T>* node) {

	// first find the only child (if any) of "node"
	RBNode<K,T> *child;
	if(node->left) {
		child = node->left;
		// make sure the node does not have two children
		assert(child->right == NULL);
	}
	else {
		// might still be NULL, meaning we are plucking a leaf node
		child = node->right;
	}


	// adjust the appropriate child pointer of the node's parent
	if(node->parent == NULL) {
		// in this case, we are deleting the root node
		// so set the new root to the child
		this->root = child;
	}
	else if(node->parent->left == node) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}

	// if we are not deleting a leaf, the child also gets a new parent
	if(child) {
		child->parent = node->parent;
	}
	//cout << "" << endl;

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


	// ensures ~RBNode() does not recursively delete other parts of the tree
	node->left = node->right = NULL;



	// the node is now gone!
	delete node;
	--RBSize;

}

// template <typename K, typename T>
// void RBTree<K,T>::fixUp(RBNode<K,T> *node) {
// 	// keep climbing up the tree until we are past the root
// 	while (node != NULL) {
// 		// first make sure the height of node is correctly computed
// 		node->recalcHeight();
//
// 		// now compare the heights of the children
// 		int lh, rh;
// 		node->childHeights(lh, rh);
//
// 		// should never differ by more than 2, otherwise
// 		// there was a bug in the code
// 		assert(abs(lh-rh) <= 2);
//
// 		// if there is a violation of the AVL property, perform the
// 		// appropriate rotation(s)
// 		// see eClass notes for the different rules for applying rotations
// 		if (lh == rh+2) {
// 			// left child is higher
//
// 			RBNode<K,T>* lchild = node->left;
// 			int llh, lrh;
// 			lchild->childHeights(llh, lrh);
//
// 			if (llh < lrh) {
// 				rotateLeft(lchild);
// 			}
// 			node = rotateRight(node);
// 		}
// 		else if (lh+2 == rh) {
// 			// right child is higher
//
// 			RBNode<K,T>* rchild = node->right;
// 			int rlh, rrh;
// 			rchild->childHeights(rlh, rrh);
//
// 			if (rlh > rrh) {
// 				rotateRight(rchild);
// 			}
// 			node = rotateLeft(node);
// 		}
//
// 		// whether we rotated or not, "node" is now the
// 		// root of the subtree we were checking
//
//
// 		// crawl up the tree one step
// 		node = node->parent;
// 	}
// }

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::rotateRight(RBNode<K,T>* node) {
	RBNode<K,T> *lchild = node->left;
	assert(left != NULL);

	// To track all of these changes, it is best to
	// draw your own picture with all relevant pointers
	// to see these operations change them properly.
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

	lchild->parent = node->parent;
	node->parent = lchild;
	if (lchild->right) {
		lchild->right->parent = node;
	}

	node->left = lchild->right;
	lchild->right = node;

	//node->recalcHeight();
	//lchild->recalcHeight();

	return lchild;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::rotateLeft(RBNode<K,T>* node) {
	RBNode<K,T> *rchild = node->right;
	assert(left != NULL);

	// To track all of these changes, it is best to
	// draw your own picture with all relevant pointers
	// to see these operations change them properly.

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

	rchild->parent = node->parent;
	node->parent = rchild;
	if (rchild->left) {
		rchild->left->parent = node;
	}

	node->right = rchild->left;
	rchild->left = node;

	//node->recalcHeight();
	//rchild->recalcHeight();

	return rchild;
}


// Helper functions
template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getParent(RBNode<K,T>* node) {
  return node->parent;
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getGrandparent(RBNode<K,T>* node) {
  RBNode<K,T>* parent = getParent(node);

  return getParent(parent);
}

template <typename K, typename T>
RBNode<K,T>* RBTree<K,T>::getSibling(RBNode<K,T>* node) {
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
  if(node->right == NULL && node->left == NULL) {
    return true;
  }
  return false;
}




// Functions for fixing tree upon node insertion
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
  // std::cout << "iCase4Breakpoint0\n";
  // Declare the relations
  RBNode<K,T>* parent = getParent(node);
  RBNode<K,T>* grandparent = getGrandparent(node);
  // std::cout << "iCase4Breakpoint1\n";

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
  // std::cout << "iCase4Breakpoint2\n";

  // This likely violates tree properties, so fix the problems the
  // rotations casued.
  insertCase4Fix(node);
  // std::cout << "iCase4Breakpoint3\n";
}

template <typename K, typename T>
void RBTree<K,T>::insertCase4Fix(RBNode<K,T>* node) {
  /* This function fixes the properties now violated by rotating the node's
   * grandparent and changing the colour of the parent and grandparent
   */
  // std::cout << "iCase4Breakpoint4\n";
  // Declare the relations
  RBNode<K,T>* parent = getParent(node);
  RBNode<K,T>* grandparent = getGrandparent(node);
  // std::cout << "iCase4Breakpoint5\n";

  // If the node is a left child, you have to rotate right
  if (node == parent->left) {
    // std::cout << "rotateRightBreakpoint\n";
    rotateRight(grandparent);
  }
  else {
    // std::cout << "rLeftBreakpoint\n";
    rotateLeft(grandparent);
  }

  // std::cout << "iCase4Breakpoint6\n";

  // Fix the colours
  parent->colour = BLACK; //true == black
  grandparent->colour = RED; //false == red

  // std::cout << "iCase4Breakpoint7\n";
}

template <typename K, typename T>
void RBTree<K,T>::fixTreeInsert(RBNode<K,T>* node) {
  /* Inputs: The node to verify the properties of
   * Output: None
   * This function validates and fixes the properties of the RBTree.
   */

  // Case 1 - If the node is the root, ensure it is black
  if (getParent(node) == NULL) {
		// cout << "Case 1 Triggered." << endl;
    insertCase1(node);
  }
  // Case 2 - If the node is not the root, ensure the parent is black
  else if (getParent(node)->colour == BLACK) {
		// cout << "Case 2 Triggered." << endl;
    return; // Since the inserted node is red, the tree properties are
            // satisfied, kill before anything is broken.
  }
  // Case 3 - The parent and uncle are not black, so simply change colours
  else if (getUncle(node) != NULL && getUncle(node)->colour == RED) {
		// cout << "Case 3 Triggered." << endl;
    insertCase3(node);
  }
  // Case 4 - Every other case
  else {
		// cout << "Case 4 Triggered." << endl;
    insertCase4(node);
  }
}


// Functions for fixing tree upon node removal
template <typename K, typename T>
void RBTree<K,T>::removeCase1(RBNode<K,T>* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  if (node->parent != NULL) {
    removeCase2(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase2(RBNode<K,T>* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  if(sibling->colour == RED) { //false == red
    node->parent->colour = RED; //false == red
    sibling->colour = BLACK; //true == black

    if (node == node->parent->left) {
      rotateLeft(node->parent);
    }
    else {
      rotateRight(node->parent);
    }
  }
  removeCase3(node);
}

template <typename K, typename T>
void RBTree<K,T>::removeCase3(RBNode<K,T>* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  if((node->parent->colour == BLACK)    && //true == black
     (sibling->colour == BLACK)         &&
     (sibling->left->colour == BLACK)   &&
     (sibling->right->colour == BLACK)) {

    sibling->colour = RED; //false == red

    removeCase1(node->parent);
  }
  else {
    removeCase4(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase4(RBNode<K,T>* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  if((node->parent->colour == RED)   && //false == red
     (sibling->colour == BLACK)         && //true == black
     (sibling->left->colour == BLACK)   &&
     (sibling->right->colour == BLACK)) {
    sibling->colour = RED;
    node->parent->colour = BLACK;
  }
  else {
    removeCase5(node);
  }
}

template <typename K, typename T>
void RBTree<K,T>::removeCase5(RBNode<K,T>* node) {
/* Inputs: node - The node with violated RBTree properties to fix
 * Output: None
 *
 */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  // This if statement is trivial,since no red parent can have a red child
  if(sibling->colour == BLACK) { //true == black
    // The following statements force the red to be on the left of the left
    // of the parent,or right of the right, so case six will rotate correctly
    if((node == node->parent->left)      &&
       (sibling->right->colour == BLACK)  &&
       (sibling->left->colour == RED)) { // this test is trivial due to cases 2-4
      sibling->colour = RED;
      sibling->left->colour = BLACK;
      rotateRight(sibling);
    }
    else if((node == node->parent->right)      &&
            (sibling->left->colour == BLACK)    &&
            (sibling->right->colour == RED)) { // this test is trivial due to cases 2-4
      sibling->colour = RED;
      sibling->right->colour = BLACK;
      rotateLeft(sibling);
    }
  }
  removeCase6(node);
}

template <typename K, typename T>
void RBTree<K,T>::removeCase6(RBNode<K,T>* node) {
  /* Inputs: node - The node with violated RBTree properties to fix
   * Output: None
   *
   */

  // Initialize the sibling
  RBNode<K,T>* sibling = getSibling(node);

  sibling->colour = node->parent->colour;
  node->parent->colour = BLACK; //true == black

  if(node == node->parent->left) {
    sibling->right->colour = BLACK; //true == black
    rotateLeft(node->parent);
  }
  else {
    sibling->left->colour = BLACK; //true == black
    rotateRight(node->parent);
  }
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
