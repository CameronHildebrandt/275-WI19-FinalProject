#include "heap.h"
#include "heap.cpp"
#include <iostream>
#include <vector>
#include <utility>
#include <time.h>
#include <algorithm>
using namespace std;

void heapsort(int array[], int size) {
    BinaryHeap<int, int> temp;
    for (int i=0; i<size; i++) {
        temp.insert(i,array[i]);
    }
    pair<int,int> value;
    for (int i=0; i<size; i++) {
        value=temp.min();
//        cout<<value.first<<" "<<value.second<<endl;
        temp.popMin();
        array[i]=value.second;
    }
}

int main() {
    int size=3000;
    int array[size];
    srand(time(NULL));
    for (int i=size-1; i>=0; i--) {
        array[i]=rand()%10000000;
    }
    for (int i=0; i<size; i++) {
        cout<<array[i]<<endl;
    }
    heapsort(array, size);
//    sort(array, array+size);
    cout<<"done"<<endl;
    for (int i=0; i<size; i++) {
        cout<<array[i]<<endl;
    }
    return 0;
}