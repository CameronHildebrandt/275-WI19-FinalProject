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

void heapsort(int array[], int size) {
    BinaryHeap<int, int> temp;
    for (int i=0; i<size; i++) {
        temp.insert(i,array[i]);
    }
    pair<int,int> value;
    for (int i=0; i<size; i++) {
        value=temp.min();
        temp.popMin();
        array[i]=value.second;
    }
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


    srand(time(NULL));
    while(true) {
        int size;
        cout<<"New tree!\nEnter the number of values in this tree:"<<endl;
        cin >> size;
        if (size==-1) {
            cout<<"Exiting"<<endl;
            break;
        }
        int array[size];
        cout<<"Now enter the values:\n";

        for (int i = 0; i < size; i++) {
            cout << size - i << " numbers left" << endl;
            cin >> array[i];
        }
        string mode;
        BinaryHeap<int, int> thing;
        for (int i = 0; i < size; i++) {
            thing.insert(i, array[i]);
        }
        cout << "hello" << endl;
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
            temp.insert(2, to_string(thing.heap[i].second));
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
    return 0;
}