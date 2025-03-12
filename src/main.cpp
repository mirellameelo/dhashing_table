#include <iostream>
#include <cmath>
#include "node.h"

static const uint16_t RING_SIZE = (1 << BITLENGTH);

void printRing(Node* startNode) {
    if (!startNode) return;
    
    Node* current = startNode;
    do {
        std::cout << "Node " << (int)current->getId() 
                  << " -> Successor: " << (int)current->getSuccessor()->getId() 
                  << " | Predecessor: " 
                  << (current->getPredecessor() ? std::to_string(current->getPredecessor()->getId()) : "None") 
                  << std::endl;
        current = current->getSuccessor();
    } while (current != startNode);  // Stop when we've completed a full cycle

    std::cout << "============================\n";
}

void testLookups(Node* startNode) {
    uint8_t keysToLookup[] = {3, 29, 40, 100, 150, 180, 240};

    std::cout << "\n=== Testing Key Lookups ===\n";
    for (uint8_t key : keysToLookup) {
        Node* responsibleNode = startNode->find_successor(key);
        std::cout << "Key " << (int)key << " is stored at Node " 
                  << (int)responsibleNode->getId() << "\n";
    }
    std::cout << "============================\n";
}

int main() {
    // 1. Create nodes.
    Node* n0 = new Node(0);
    Node* n1 = new Node(30);
    Node* n2 = new Node(65);
    Node* n3 = new Node(110);
    Node* n4 = new Node(160);
    Node* n5 = new Node(230);

    // 2. Join the ring.
    n0->join(nullptr);  // First node
    n1->join(n0);
    n2->join(n1);
    n3->join(n2);
    n4->join(n3);
    n5->join(n4);

    // 3. Print the initial ring structure
    std::cout << "\n=== Ring Structure After Joining ===\n";
    printRing(n0);

    // 4. Stabilize all nodes multiple times
    for (int i = 0; i < 5; i++) {
        n0->stabilize();
        n1->stabilize();
        n2->stabilize();
        n3->stabilize();
        n4->stabilize();
        n5->stabilize();
    }

    // 5. Print ring structure after stabilization
    // std::cout << "\n=== Ring Structure After Stabilization ===\n";
    // printRing(n0);

    //OKAY UP TO HERE

    // 6. Run fix_fingers() multiple times
    for (int i = 0; i < 5; i++) {
        n0->fix_fingers();
        n1->fix_fingers();
        n2->fix_fingers();
        n3->fix_fingers();
        n4->fix_fingers();
        n5->fix_fingers();
    }

    // 7. Print each node’s finger table
    std::cout << "\n=== Finger Tables After fix_fingers() ===\n";
    n0->print_finger_table();
    n1->print_finger_table();
    n2->print_finger_table();
    n3->print_finger_table();
    n4->print_finger_table();
    n5->print_finger_table();


    // 8. Test key lookups
    testLookups(n0);

    // 8. Clean up
    delete n0;
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;

    return 0;
}