// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <cassert>
#include <iostream>
#include <cstdlib>
#include "RBTreeAux.h"

using namespace std;

enum Colour {RED, BLACK};

/*
  An associative container (map/dict) using an AVL tree.
	The update, remove, [], at, and hasKey operations take O(log n)
	time (using O(log n) comparisons) where n = # entries
	in the tree.

  Assumes:
    - K is totally ordered and can be compared via <
*/

template <typename K, typename T>
class RBTree {
public:
	// creates an empty RBTree with 0 items
	RBTree();

	// deletes all nodes in the RBTree
	~RBTree();

	// add the item with the given key, replacing
	// the old item at that key if the key already exists
	void update(const K& key, const T& item);

	// remove the key and its associated item
	void remove(const K& key);

	// returns true iff the key exists
	bool hasKey(const K& key) const;

	// access the item at the given key, allows assignment
	// as an l-value, eg. tree["Zac"] = 20;
	// where tree is an instance of RBTree<string, int>
	T& operator[](const K& key);

	// does not create the entry if it does not exist
	const T& at(const K& key) const;

	// returns the size of the tree
	unsigned int size() const;

	// returns an iterator to the first item (ordered by key)
	RBIterator<K,T> begin() const;

	// returns an iterator signalling the end iterator
	RBIterator<K,T> end() const;

	// will run a check ensuring the AVL tree is still well-formed
	// (i.e. is a binary search tree with the AVL property at each node
	// and has the correct node count)
	// Takes O(n) time.
	void checkStructure() const;
	RBNode<K,T> *root;

private:

	unsigned int RBSize;

	// returns a pointer to the node containing the key,
	// or to what its parent node would be if the key does not exist,
	// or NULL if the tree is currently empty
	RBNode<K,T>* findNode(const K& key) const;

	// assumes at least one child of node is NULL, will delete
	// the node and move its only child (if any) to its place
	void pluckNode(RBNode<K,T>* node);

	// fix the AVL property at this node and, recursively, all
	// nodes above it
//	void fixUp(RBNode<K,T>* node);

	// recalculate the height of the node, assuming its children's heights
	// are correct
//	void recalcHeight(RBNode<K,T>* node);

	// left and right rotations, return a pointer to the new root
	// of the subtree after the rotation, assumes the corresponding
	// node->left or node->right are not null
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


	// Functions for fixing tree upon node removal
	void removeCase1(RBNode<K,T>* node);

	void removeCase2(RBNode<K,T>* node);

	void removeCase3(RBNode<K,T>* node);

	void removeCase4(RBNode<K,T>* node);

	void removeCase5(RBNode<K,T>* node);

	void removeCase6(RBNode<K,T>* node);

	void replaceNode(RBNode<K,T>* node, RBNode<K,T>* child);

	void removeChild(RBNode<K,T>* node);


	// helper function for checkStructure
	unsigned int checkStructureRecursive(RBNode<K,T> *node,
		RBNode<K,T> *parent, RBNode<K,T> *lb, RBNode<K,T> *ub) const;
};

// ***** IMPLEMENTATION STARTS HERE *****
// ***** PUBLIC METHODS FIRST *****

template <typename K, typename T>
RBTree<K,T>::RBTree() {
	this->root = NULL;
	this->RBSize = 0;
}

template <typename K, typename T>
RBTree<K,T>::~RBTree() {
	if (this->root != NULL) {
		delete this->root;

		// point to NULL
		this->root = NULL;


		this->RBSize = 0;
	}
}

template <typename K, typename T>
void RBTree<K,T>::update(const K& key, const T& item) {
	RBNode<K,T>* node = findNode(key);

	// if there was no node in the tree with this key, create one
	if (node == NULL || node->key != key) {
		RBNode<K,T> *newNode = new RBNode<K,T>(key, item, NULL, NULL, node, RED);
		assert(newNode != NULL);

		// change the left or right pointer of the parent node
		// whichever is appropriate to preserve the AVL property
		if (node == NULL) {
			// the tree was empty, so this is the new root
			root = newNode;
		}
		else {
			// the tree was not empty, so put it as the appropriate child of "node"
			if (key < node->key) {
				node->left = newNode;
			}
			else {
				node->right = newNode;
			}
		}
		++RBSize;

		// now fix the AVL property up the tree
		fixTreeInsert(newNode);
	}

	else {
		// the key existed, so just update the item
		node->item = item;
	}

}

template <typename K, typename T>
void RBTree<K,T>::remove(const K& key) {
	RBNode<K,T>* node = findNode(key);

	// make sure the key is in the tree
	// we only assume < is implemented for the key type, not necessarily ==
//	assert(node != NULL && !(node->key < key || key < node->key));
	if(node == NULL && !(node->key < key || key < node->key)) {
		cout << "rip search" << endl;
		return;
	}
	// find the maximum-key node in the left subtree of the node to remove
	RBNode<K,T> *tmp = node->left, *pluck = node;
	while (tmp != NULL) {
		pluck = tmp;
		tmp = tmp->right;
	}
	// pluck is the one to remove, it might be pluck == node if node has
	// no left child

	// copy contents from node we are deleting
	node->key = pluck->key;
	node->item = pluck->item;

//	RBNode<K,T> *pluckParent = pluck->parent;

	//removeChild(pluck->parent);
	cout<<"breakpoint0\n";

	// this function will delete a node with no left child and
	// restructure the tree
	pluckNode(pluck);

	// now fix the AVL tree up starting from the parent
	// of the recently-deleted node
//	fixUp(pluckParent);

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
const T& RBTree<K,T>::at(const K& key) const {
	const RBNode<K,T> *node = findNode(key);
	assert(node != NULL && !(node->key != key));

	return node->item;
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
	if (node->parent == NULL) {
		// in this case, we are deleting the root node
		// so set the new root to the child
		this->root = child;
	}
	else if (node->parent->left == node) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}

	// if we are not deleting a leaf, the child also gets a new parent
	if (child) {
		child->parent = node->parent;
	}

	// Fix the RB properties
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

	// the node is now gone!
	delete node;
	--RBSize;
}

//template <typename K, typename T>
//void RBTree<K,T>::fixUp(RBNode<K,T> *node) {
//	// keep climbing up the tree until we are past the root
//	while (node != NULL) {
//		// first make sure the height of node is correctly computed
//		node->recalcHeight();
//
//		// now compare the heights of the children
//		int lh, rh;
//		node->childHeights(lh, rh);
//
//		// should never differ by more than 2, otherwise
//		// there was a bug in the code
//		assert(abs(lh-rh) <= 2);
//
//		// if there is a violation of the AVL property, perform the
//		// appropriate rotation(s)
//		// see eClass notes for the different rules for applying rotations
//		if (lh == rh+2) {
//			// left child is higher
//
//			RBNode<K,T>* lchild = node->left;
//			int llh, lrh;
//			lchild->childHeights(llh, lrh);
//
//			if (llh < lrh) {
//				rotateLeft(lchild);
//			}
//			node = rotateRight(node);
//		}
//		else if (lh+2 == rh) {
//			// right child is higher
//
//			RBNode<K,T>* rchild = node->right;
//			int rlh, rrh;
//			rchild->childHeights(rlh, rrh);
//
//			if (rlh > rrh) {
//				rotateRight(rchild);
//			}
//			node = rotateLeft(node);
//		}
//
//		// whether we rotated or not, "node" is now the
//		// root of the subtree we were checking
//
//
//		// crawl up the tree one step
//		node = node->parent;
//	}
//}

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

//	node->recalcHeight();
//	lchild->recalcHeight();

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

//	node->recalcHeight();
//	rchild->recalcHeight();

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
    node->colour = BLACK; //true == black
  }
}

template <typename K, typename T>
void RBTree<K,T>::insertCase3(RBNode<K,T>* node) {
  /* This function ensures that red nodes do not have red children
   */

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

  // std::cout << "fixBreakpoint0\n";
  // Case 1 - If the node is the root, ensure it is black
  if (getParent(node) == NULL) {
    // std::cout << "fixBreakpoint1\n";
    insertCase1(node);
  }
  // Case 2 - If the node is not the root, ensure the parent is black
  else if (getParent(node)->colour == BLACK) {
    // std::cout << "fixBreakpoint2\n";
    return; // Since the inserted node is red, the tree properties are
            // satisfied.
  }
  // Case 3 - The parent and uncle are not black, so simply change colours
  else if (getUncle(node) != NULL && getUncle(node)->colour == RED) {
    // std::cout << "fixBreakpoint3\n";
    insertCase3(node);
  }
  // Case 4 - The parent is not black, but the uncle is, so start rotating
  else if(getUncle(node) != NULL && getUncle(node)->colour == BLACK){
    // std::cout << "fixBreakpoint4\n";
    insertCase4(node);
  }
  // std::cout << "fixBreakpoint5\n";
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
     std::cout << "Error: Attempted to remove a node without a child " <<
                  "that is a leaf node.\n";
      return;
   }

  // Select the child that is not a leaf
  RBNode<K,T>* child = isLeaf(node->right) ? node->left : node->right;

  // Replace the current node
  replaceNode(node, child);

  // If the node is black and the child is red, make the child black
  if (node->colour == BLACK) { //true == black
    if (child->colour == RED) { //false == red
      child->colour = BLACK; //true == black
    }
  else
    removeCase1(child);
  }

	// ensures ~RBNode() does not recursively delete other parts of the tree
	node->left = node->right = NULL;

	// the node is now gone!
	delete node;
	--RBSize;

//  // Remove node from memory
//  free(node);
}




template <typename K, typename T>
void RBTree<K,T>::checkStructure() const {
	// if (root == NULL) {
	// 	assert(RBSize == 0);
	// }
	// else {
	// 	assert(root->parent == NULL);
	// 	unsigned int checkSize = checkStructureRecursive(root, NULL, NULL, NULL);
	// 	assert(checkSize == RBSize);
	// }
}

// Recursively:
// - checks the AVL height property in all nodes the subtree rooted at "node"
// - calculates and returns the number of nodes in this subtree
// - the height of the nodes are correct
// - ensures the BST property at each node in the subtree holds, see
//   the inline comments to understand how this is done
template <typename K, typename T>
unsigned int RBTree<K,T>::checkStructureRecursive(RBNode<K,T> *node,
	RBNode<K,T> *parent, RBNode<K,T> *lb, RBNode<K,T> *ub) const {

	unsigned int cnt = 1;

	// Think: lb means "lower bound" and ub means "upper bound".
	//   lb is the deepest node above "node" such that "node" is
	//   in the right subtree of lb, similarly for ub (except left subtree)
	//   So if node->key and some key in a node above "node" violate the BST
	//   but no violation has been detected so far earlier in the recursion
	//   then it node->key would violate the BST property with one of ub or lb

	// if (lb != NULL) {
	// 	assert(lb->key < node->key);
	// }
	// if (ub != NULL) {
	// 	assert(node->key < ub->key);
	// }
	//
	// // the following recursively checks
	// if (node->left) {
	// 	assert(node->left->parent == node);
	// 	// replace ub with "node" in the recursive check as we went left
	// 	cnt += checkStructureRecursive(node->left, node, lb, node);
	// }
	// if (node->right) {
	// 	assert(node->right->parent == node);
	// 	// replace lb with "node" in the recursive check as we went right
	// 	cnt += checkStructureRecursive(node->right, node, node, ub);
	// }
	//
	// // now check the heights and the AVL property
	// int lh, rh;
	// node->childHeights(lh, rh);
	// assert(node->height == 1+max(lh, rh));
	// assert(abs(lh-rh) <= 1);

	return cnt;
}

#endif
