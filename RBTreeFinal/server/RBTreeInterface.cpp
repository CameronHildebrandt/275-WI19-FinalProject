// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include <iostream>
#include <string>
#include "RBTree.h"
#include "serialport.cpp"
#include "serialport.h"

// void printTree(const RBTree<int, string>& tree) {
//   for (RBIterator<int, string> iter = tree.begin(); iter != tree.end(); ++iter) {
//     cout << "- " << iter.key() << ' ' << iter.colour() << endl;
//   }
// }

SerialPort Serial("/dev/ttyACM0");

int main() {
  RBTree<int, string> tree;
  string temp;

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
  string out;
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
    string word;

    cin >> cmd;
    switch(cmd) {
    case 'S':
        cout << tree.size() << endl;
        break;

    case 'I':
      word = "null";
      cin >> value;
      tree[value] = word;
      //printTree(tree);
      tree.treePrint();
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
      else {
        tree.remove(value);
      }
      break;

    case 'P':
      //cout << "printing" << endl;
      // printTree(tree);


      temp=tree.treePrint();
      cout<<temp<<endl;
      for (auto x: temp) {
        if(x==' ') {
          continue;
        }
        out.clear();
        out+=x;
        out+="\n";
        cout<<out;
        do {
          Serial.writeline(out);
        } while(Serial.readline()!="A\n");
        cout<<"ok"<<endl;
      }
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

    // tree.checkStructure();
  }
}
