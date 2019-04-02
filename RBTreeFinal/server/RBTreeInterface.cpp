// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include <iostream>
#include <string>
#include "RBTree.h"
#include "serialport.h"
#include "serialport.cpp"
#include <vector>
#include <algorithm>

SerialPort Serial("/dev/ttyACM0");


void serialPrintTree(vector<int> vec, int size) {
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
    temp.insert(2, to_string(vec[i]));
    temp.insert(temp.size(), "\n");
//    cout << temp << endl;
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

void printTree(const RBTree<int, int>& tree) {
  int index=0;
  vector<int> temp;
  for (int i=0; i<50; i++) {
    temp.push_back(0);
  }
  for (RBIterator<int, int> iter = tree.root; iter != tree.end(); ++iter) {
    if (iter==tree.root) {
      temp.insert(temp.begin(), iter.item());
    }
    else {
      RBNode<int,int> *parent= iter.node->parent;
      int parentitem=parent->item;
      cout<<parentitem;
      vector<int>::iterator position=find(temp.begin(),temp.end(), parentitem);
      if (iter==parent->left) {
        index=distance(temp.begin(), position)*2+1;
      }

      if (iter==parent->right) {
        index=distance(temp.begin(), position)*2+2;
      }
      temp[index]=iter.item();
    }
    if (!iter.node->left) {
      temp[index*2+1]=0;
    }
    if (!iter.node->right) {
      temp[index*2+2]=0;
    }
    cout << " - " << iter.key() << ' ' << iter.item() << ' ' << iter.colour() << endl;
  }
  serialPrintTree(temp, temp.size());
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

  RBTree<int, int> tree;

  cout << "Possible Commands:" << endl;
  cout << "I <value> - insert a value" << endl;
  cout << "F <value> - check if the value is in the tree" << endl;
  cout << "R <value> - remove the entry with the given value" << endl;
  cout << "S         - print the size of the map" << endl;
  cout << "P         - print all entries in the tree, ordered by key" << endl;
  cout << "Q         - stop" << endl;
  cout << endl;

  while (true) {
    char cmd;
    int value;
    int word;

    cin >> cmd;
    switch(cmd) {
    case 'S':
        cout << tree.size() << endl;
        break;

    case 'I':
      word = tree.size();
      cin >> value;
      tree[word] = value;
      break;

    case 'F':
      cin >> value;
      if (tree.hasKey(value)) {
        cout << value << " found!" << endl;
      }
      else {
        cout << value << " not found." << endl;
      }
      break;

    case 'R':
      cin >> value;
      if (!tree.hasKey(value)) {
        cout << value << " not found" << endl;
      }
      tree.remove(value);
      break;

    case 'P':
      cout << "printing" << endl;
      printTree(tree);
      break;

    case 'Q':
      cout << "stopping..." << endl;
      return 0;

    default:
      cout << "invalid command." << endl;
      // // eat up the rest of the line
      // getline(cin, value);
      break;
    }

    tree.checkStructure();
  }
}
