// --------------------------------------------------------
// | Name: Ramana Vasanthan, 1458497
// | Name: Cameron Hildebrandt, 1584696
// | CMPUT 275, Winter 2019
// | Assignment #2, Part 2 - Driving Route Finder
// --------------------------------------------------------

#include "heap.h"

// get the root of the heap
template <class T, class K>
pair<T, K> BinaryHeap<T,K>::min() const {
    return heap.front();
}

// insert new value and maintain heap property
template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key) {
    pair<T, K> v;
    int index;
    int parentindex;
    v.first=item;
    v.second=key;
    heap.push_back(v);
    index=heap.size()-1;
    parentindex=(index-1)/2;


    while(v!=heap.front() and v.second<heap[parentindex].second) {
        swap(heap[index], heap[parentindex]);
        v=heap[parentindex];
        index=parentindex;
        parentindex=(index-1)/2;
    }

}

// pop the minimum item from the heap and maintains heap property
template <class T, class K>
void BinaryHeap<T,K>::popMin() {
    if (heap.size()==0) {
        cout<<"Error: heap is empty"<<endl;
        return;
    }
    pair<T, K> r;
    swap(heap.front(),heap.back());
    heap.pop_back();
    unsigned int index=0;
    unsigned int childindex=0;

    // initialize child numbering
    if (heap.size()>=3) {

        if (heap[1].second < heap[0].second or heap[2].second < heap[0].second) {
            if (heap[1].second < heap[2].second) {
                childindex = 1;
            } else {
                childindex = 2;
            }
        }
    }
    else if (heap.size()==2) {
        if (heap[1].second<heap[0].second) {
            swap(heap[0],heap[1]);
        }
    }
    else {
        return;
    }
    // maintain binary heap property
    while (heap[childindex].second<heap[index].second and childindex<=heap.size()-1) {
        swap(heap[index], heap[childindex]);
        index=childindex;
        if (heap[index*2+1].second<heap[index].second or heap[index*2+2].second<heap[index].second) {
            if (heap[index*2+1].second<heap[index*2+2].second) {
                childindex=index*2+1;
            }
            else {
                childindex=index*2+2;
            }
        }
    }

}

// returns the number of items held in the heap
template <class T, class K>
int BinaryHeap<T,K>::size() const {
    return heap.size();
}
