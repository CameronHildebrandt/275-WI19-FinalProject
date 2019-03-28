// --------------------------------------------------------
// | Name: Ramana Vasanthan, 1458497
// | Name: Cameron Hildebrandt, 1584696
// | CMPUT 275, Winter 2019
// | Assignment #2, Part 2 - Driving Route Finder
// --------------------------------------------------------


#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include <utility> // for pair
#include <iostream>

using namespace std;

// T is the type of the item to be held
// K is the type of the key associated with each item in the heap
// The only requirement is that K is totally ordered and comparable via <
template <class T, class K>
class BinaryHeap {
public:
  // constructor not required because the only "initialization"
  // is through the constructor of the variable "heap" which is called by default

  // return the minimum element in the heap
  pair<T, K> min() const;

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key);

  // pop the minimum item from the heap
  void popMin();

  // returns the number of items held in the heap
  int size() const;

private:
  // the array holding the heap
  vector< pair<T, K> > heap;
  // feel free to add appropriate private methods to help implement some functions
};

#endif
