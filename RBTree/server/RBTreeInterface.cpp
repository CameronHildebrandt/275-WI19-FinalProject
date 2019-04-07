// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

// Include the required libraries
#include <iostream>
#include <string>
#include "RBTree.h"

// Libraries for Arduino (not inluded, see below)
// #include "serialport.cpp"
// #include "serialport.h"

// Connect the Arduino
// SerialPort Serial("/dev/ttyACM0");

int main() {
  // Initialize the data structure
  RBTree<int, string> tree;

  // Initialize the variables
  char cmd;
  int key;

  // The else case goes through characters one by one and would print out
  // "invalid command" for every character. This flag ensures that only one
  // is printed until a
  bool invalidFlag = true;

  // Code used for communicating with Arduino.
  // We were not able to get the tree to draw properly, so to make running the
  // code easier these lines aren't included so the RBTree can be run as a
  // desktop app.
  /*
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
  */

  // Print the user guide
  cout << "Possible Commands:" << endl;
  cout << "I <key> - insert a key" << endl;
  cout << "F <key> - find a key" << endl;
  cout << "R <key> - remove a key" << endl;
  cout << "S       - print the size of the tree" << endl;
  cout << "P       - print the tree, order defined in the README" << endl;
  cout << "Q       - kill the program" << endl;
  cout << endl;

  // Wait for commands
  while (true) {

    // Read in the command
    cin >> cmd;

    if(cmd == 'S' || cmd == 's') { // Print the size of the tree
      cout << tree.size() << endl;
      invalidFlag = true; // Reset the flag
    }

    else if(cmd == 'I' || cmd == 'i') { // Insert a key
      cin >> key;

      // The tree expects a string value to append to every key as well.
      // Since we don't use the string value, we just set it to a placeholder
      // value.
      tree[key] = "null";
      tree.treePrint();
      invalidFlag = true; // Reset the flag
    }

    else if(cmd == 'F' || cmd == 'f') { // Find a key
      cin >> key;

      if (tree.hasKey(key)) {
        cout << key << " found!" << endl;
      }
      else {
        cout << key << " not found." << endl;
      }
      invalidFlag = true; // Reset the flag
    }

    else if(cmd == 'R' || cmd == 'r') { // Remove a key
      cin >> key;

      if (!tree.hasKey(key)) {
        cout << key << " not found" << endl;
      }
      else {
        tree.remove(key);
        tree.treePrint();
      }
      invalidFlag = true; // Reset the flag
    }

    else if(cmd == 'P' || cmd == 'p') { // Print the tree
      // Code used for the communication with the Arduino
      // (not included, see above)
      /*
      out.clear();
      temp=tree.treePrint();
      cout<<temp<<endl;
      for (auto x: temp) {
        out+=x;
        out+="\n";
        do {
          Serial.writeline(out);
        } while(Serial.readline()!="A\n");
      }
      */

      // Print the tree
      tree.treePrint();
      invalidFlag = true; // Reset the flag
    }

    else if(cmd == 'Q' || cmd == 'q') { // Kill the program
      cout << "stopping..." << endl;
      return 0;
    }

    else { // Invalid command entered
      if(invalidFlag) {
        cout << "invalid command." << endl;
        invalidFlag = false;
      }
    }
  }
}
