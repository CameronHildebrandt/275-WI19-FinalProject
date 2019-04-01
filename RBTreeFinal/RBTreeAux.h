// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#ifndef _RBTREE_AUX_H_
#define _RBTREE_AUX_H_

#include <cstdlib>

// forward declaration of class, so RBNode can establish it's "friends" :)
template <typename K, typename T> class RBTree;
template <typename K, typename T> class RBIterator;

/*
Node for holding the key, item, and pointers for a node in the AVL.
Everything is private, only RBTree abd RBIterator have access.
*/
template <typename K, typename T>
class RBNode {
private:
  RBNode(const K& key, const T& item,
    RBNode<K,T>* left, RBNode<K,T>* right, RBNode<K,T>* parent, bool colour) {
      this->key = key;
      this->item = item;
      this->colour = colour;
      this->left = left;
      this->right = right;
      this->parent = parent;
      recalcHeight();
  }

  // recursively delete the left and right subtree, if they exist
  ~RBNode() {
    if (this->left) {
      delete this->left;
    }
    if (this->right) {
      delete this->right;
    }
  }

  // recalculate the height of this node
  // assumes the heights of the children are correct
  void recalcHeight() {
    int lh, rh;
    childHeights(lh, rh);
    height = 1+std::max(lh, rh);
  }

  // get the heights of the children
  void childHeights(int& lh, int& rh) {
    lh = rh = -1;
    if (left) {
      lh = left->height;
    }
    if (right) {
      rh = right->height;
    }
  }

  K key;
  T item;
  bool colour;
  RBNode<K,T> *left, *right, *parent;
  int height;

  // give access to the AVL map class itself and its iterators
  friend class RBTree<K,T>;
  friend class RBIterator<K,T>;
};

/*
Iterator class for the RBTree class.

Supports:
- key()
- item(), as an l-value as well
- prefix and postfix increment
- == and !=
*/
template <typename K, typename T>
class RBIterator {
public:
  const K& key() const {
    return this->node->key;
  }

  // allows assignment to the item, eg. iter.item() = 17
  // this will update the item held at by the corresponding key

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

  // prefix operator: ++iter
  RBIterator<K,T> operator++() {
    advance();
    return *this;
  }

  // postfix operator: iter++
  RBIterator<K,T>& operator++(int) {
    // uses the default copy constructor to copy this->node
    RBIterator<K,T> tmp(*this);
    advance();
    return tmp;
  }

  bool operator==(const RBIterator<K,T>& rhs) const {
    return node == rhs.node;
  }

  bool operator!=(const RBIterator<K,T>& rhs) const {
    return node != rhs.node;
  }

private:
  RBIterator(RBNode<K,T> *root) {
    this->node = root;
    if (node != NULL) {
      // if the root of the tree is not empty, go to leftmost node
      while (this->node->left) {
        this->node = node->left;
      }
    }
  }

  void advance() {
    assert(node != NULL);
    if (this->node->right) {
      // if there is a right subtree, go to its leftmost/minimum-key node
      this->node = this->node->right;
      while (this->node->left) {
        this->node = this->node->left;
      }
    }
    else {
      // crawl up parent pointers while this node is the right child of the parent
      const RBNode<K,T> *old;
      do {
        old = this->node;
        this->node = this->node->parent;
      } while (this->node != NULL && this->node->right == old);
    }
  }

  RBNode<K,T> *node;

  // needed so RBTree can access the constructor
  friend class RBTree<K,T>;
};


  #endif
