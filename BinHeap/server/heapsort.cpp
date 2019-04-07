// --------------------------------------------------------
// | Name: Ramana Vasanthan, 1458497
// | Name: Cameron Hildebrandt, 1584696
// | CMPUT 275, Winter 2019
// | Final Project- Data Structure Visualizer
// --------------------------------------------------------

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

// initialize the serial port
SerialPort Serial("/dev/ttyACM0");

void printTree(BinaryHeap<int,int> tree, int size) {

    // Mode is the type of information that will be sent to the Arduino
    string mode;

    // Temp is the whole string that will be sent
    string temp;

    // First, the number of items will be sent so N mode is used
    temp.insert(0, "N ");

    // The size of the tree is then inserted and printed
    temp.insert(2, to_string(size));
    temp.insert(temp.size(), "\n");
    cout << temp << endl;
    Serial.writeline(temp);

    // Waits until acknowledgement is received
    mode = Serial.readline();
    cout << mode << endl;

    // Send each value with the entry, E, mode and wait for acknowledgement
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

    // Use the exit mode, X, to show the end of the tree and wait for acknowledgement
    temp.insert(0, "X \n");
    do {
        Serial.writeline(temp);
    } while (Serial.readline() != "A\n");
    cout << "Done!" << endl<<endl;

}

BinaryHeap<int,int> newTree(int size) {

    // make a new tree
    BinaryHeap<int,int> tree;

    // Make an array of values to be inserted into the tree
    cout<<"New tree!\nEnter the number of values in this tree:"<<endl;
    int array[size];
    cout<<"Now enter the values:\n";

    // Get all the values and insert them into the tree
    for (int i = 0; i < size; i++) {
        cout << size - i << " numbers left" << endl;
        cin >> array[i];
        tree.insert(i,array[i]);
    }

    // Print the tree
    printTree(tree, size);
    return tree;
}


int main() {

    // Initialize the variables used for handshake exchange
    string nextPhase = "PHASE01\n";
    string line;

    // Wait until the client sends the first signal
    do {
        line = Serial.readline();
    } while (line != nextPhase);
    cout<<"Link established"<<endl;

    // switch to next phase and wait for link to be established
    while(true) {
        Serial.writeline("x");
        if (Serial.readline()=="thanks\n") {
            break;
        }
    }

    // Initialize a tree and set variables to do actions later
    BinaryHeap<int,int> tree;
    int size;
    char action;

    while(true) {

        // Get the command from standard in
        cin>>action;

        // Get the number of values to be inputted, make a new tree and print it
        if (action=='N') {
            cin>>size;
            tree=newTree(size);
        }

        // Insert a new value to the tree and print it
        else if (action=='I') {
            int newnum;
            cin>>newnum;
            tree.insert(size, newnum);
            size++;
            printTree(tree,size);
        }

        // Pop the smallest value and print the tree
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