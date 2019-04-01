// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include <iostream>
#include <string>
#include "RBTree.h"

void printTree(const RBTree<int, string>& tree) {
  for (RBIterator<int, string> iter = tree.begin(); iter != tree.end(); ++iter) {
    cout << " - " << iter.key() << ' ' << iter.item() << ' ' << iter.colour() << endl;
  }
}

int main() {
  RBTree<int, string> tree;

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
