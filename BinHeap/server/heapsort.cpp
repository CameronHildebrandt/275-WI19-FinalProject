#include "heap.h"
#include "heap.cpp"
#include <iostream>
#include <vector>
#include <utility>
#include <time.h>
#include <algorithm>
#include "serialport.h"
#include "serialport.cpp"
#include <string>
using namespace std;

SerialPort Serial("/dev/ttyACM0");

void printTree(BinaryHeap<int,int> tree, int size) {
    cout << "hello" << endl;
    string mode;
    string temp;
    temp.insert(0, "N ");
    temp.insert(2, to_string(size));
    temp.insert(temp.size(), "\n");
    cout << temp << endl;
    Serial.writeline(temp);
    mode = Serial.readline();
    cout << mode << endl;
    for (int i = 0; i < size; i++) {
        temp.clear();
        temp.insert(0, "E ");
        temp.insert(2, to_string(tree.heap[i].second));
        temp.insert(temp.size(), "\n");
        cout << temp << endl;
        do {
            Serial.writeline(temp);
        } while (Serial.readline() != "A\n");
    }
    temp.clear();
    temp.insert(0, "X \n");
    do {
        Serial.writeline(temp);
    } while (Serial.readline() != "A\n");
    cout << "Done!" << endl<<endl;

}

BinaryHeap<int,int> newTree(int size) {
    BinaryHeap<int,int> tree;
    cout<<"New tree!\nEnter the number of values in this tree:"<<endl;
    int array[size];
    cout<<"Now enter the values:\n";

    for (int i = 0; i < size; i++) {
        cout << size - i << " numbers left" << endl;
        cin >> array[i];
        tree.insert(i,array[i]);
    }
    printTree(tree, size);
    return tree;
}


int main() {
    // Initialize the variables
    string nextPhase = "PHASE01\n";
    string line;
    do {
        line = Serial.readline();
    } while (line != nextPhase);
    cout<<"Link established"<<endl;
    // switch to next phase
    while(true) {
        Serial.writeline("x");
        if (Serial.readline()=="thanks\n") {
            break;
        }
    }

    BinaryHeap<int,int> tree;
    int size;
    char action;

    while(true) {

        cin>>action;

        if (action=='N') {
            cin>>size;
            tree=newTree(size);
        }
        else if (action=='I') {
            int newnum;
            cin>>newnum;
            tree.insert(size, newnum);
            size++;
            printTree(tree,size);
        }
        else if (action=='P') {
            tree.popMin();
            size--;
            printTree(tree,size);
        }
        else {
            cout<<"Invalid command. Please retry.\n";
        }

    }
    return 0;
}