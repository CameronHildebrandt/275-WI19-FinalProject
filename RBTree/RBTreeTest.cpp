// --------------------------------------------------------
// | Name: Cameron Hildebrandt, 1584696
// | Name: Ramana Vasanthan, 1458497
// | CMPUT 275, Winter 2019
// | Final Project - Red-Black Tree Visualizer
// --------------------------------------------------------

#include "RBTree.h"

int main() {
  // Initialize the tree to store data
  RBTree tree;


  // Greet the user and define the possible commands
  std::cout << "\nCMPUT 275 Final Project: Red-Black Tree Visualizer\n\n";
  std::cout << "Commands:\n";
  std::cout << "I <value>    Insert a value (int)\n";
  std::cout << "D <value>    Delete a value (int)\n";
  std::cout << "F <value>    Find a value (int)\n";
  std::cout << "S            Kill the program\n";
  std::cout << "\n";
  std::cout << "The tree will automatically be reloaded on a connected " <<
               "Arduino after I or D commands are called.\n";
  std::cout << "\n";

  // Main program loop
  while(true) {
    // Initialize the required variables
    RBNode ogNode;
    RBNode ogRoot;
    RBNode* node = &ogNode;
    RBNode* root = &ogRoot;
    char command;
    int value = 0;

    std::cin >> command;

    // Insert value command
    if(command == 'I' || command == 'i') {
      std::cin >> value;
      node->colour = false; //false == red
      node->value = value;

      root = tree.insert(root, node);
    }

    else if(command == 'D' || command == 'd') {
      std::cin >> value;
    }

    else if(command == 'F' || command == 'f') {
      std::cin >> value;
    }

    else if(command == 'S' || command == 's') {
      std::cout << "Stopping program...\n\n";
      break;
    }

  }


  return 0;
}
