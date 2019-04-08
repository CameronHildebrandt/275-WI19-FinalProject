// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Data Structure Visualizer
// --------------------------------------------------------

#ifndef _RBTREE_AUX_H_
#define _RBTREE_AUX_H_

// Include the required libraries
#include <cstdlib>

// Forward declaration of class
template <typename K, typename T> class RBTree;
template <typename K, typename T> class RBIterator;


template <typename K, typename T>
class RBNode {
  /* This is a node of the tree. It knows its parent, left and right child,
   * its colour, and the key and item that it stores. All parts are private so
   * that can only be accessed from the RBTree and RBIterator
  */
private:
  // Constructor
  RBNode(const K& key, const T& item,
    RBNode<K,T>* left, RBNode<K,T>* right, RBNode<K,T>* parent, bool colour) {
      this->key = key;
      this->item = item;
      this->colour = colour;
      this->left = left;
      this->right = right;
      this->parent = parent;
  }

  // Destructor
  ~RBNode() {
    /* Recursively delete the left and right children to ensure the entire
     * tree is wiped from memory
     */
    if (this->left) {
      delete this->left;
    }
    if (this->right) {
      delete this->right;
    }
  }

  // Elements of the node
  K key;
  T item;
  bool colour;
  RBNode<K,T> *left, *right, *parent;

  // Give access to the friend classes
  friend class RBTree<K,T>;
  friend class RBIterator<K,T>;
};


template <typename K, typename T>
class RBIterator {
  /* Iterator class that defines how to traverse the tree
   */
public:
  const K& key() const {
    return this->node->key;
  }

  const T& item() const {
    return this->node->item;
  }

  T& item(){
    return this->node->item;
  }

  const bool& colour() const {
    return this->node->colour;
  }

  bool& colour(){
    return this->node->colour;
  }

  // Prefix operator: ++iter
  RBIterator<K,T> operator++() {
    advance();
    return *this;
  }

  // Postfix operator: iter++
  RBIterator<K,T>& operator++(int) {
    // uses the default copy constructor to copy this->node
    RBIterator<K,T> tmp(*this);
    advance();
    return tmp;
  }

  // Equivelancy operator
  bool operator==(const RBIterator<K,T>& rhs) const {
    return node == rhs.node;
  }

  // Non-equivelancy operator
  bool operator!=(const RBIterator<K,T>& rhs) const {
    return node != rhs.node;
  }

private:
  RBIterator(RBNode<K,T> *root) {
    this->node = root;
    if (node != NULL) {
      // If the root of the tree is not empty, go to leftmost node
      while (this->node->left) {
        this->node = node->left;
      }
    }
  }

  void advance() {
    assert(node != NULL);
    if (this->node->right) {
      // If there is a right subtree, go to its leftmost/minimum-key node
      this->node = this->node->right;
      while (this->node->left) {
        this->node = this->node->left;
      }
    }
    else {
      // cCrawl up parent pointers while this node is the right child of the parent
      const RBNode<K,T> *old;
      do {
        old = this->node;
        this->node = this->node->parent;
      } while (this->node != NULL && this->node->right == old);
    }
  }

  RBNode<K,T> *node;

  // Give RBTree acess to the iterator
  friend class RBTree<K,T>;
};

  #endif
