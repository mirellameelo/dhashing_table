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

    // Dynamically collect all nodes from the Chord ring and stabilize
    Node::stabilizeNetwork(n0);

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

    // 8. Insert keys into the Chord ring
    std::cout << "\n=== Inserting Keys ===\n";
    n0->insert(3, 3);
    n1->insert(200);  
    n2->insert(123);
    n3->insert(45, 3);
    n4->insert(99);
    n2->insert(60, 10);
    n0->insert(50, 8);
    n3->insert(100, 5);
    n3->insert(101, 4);
    n3->insert(102, 6);
    n5->insert(240, 8);
    n5->insert(250, 10);

    // 9. Print stored keys at each node
    std::cout << "\n=== Stored Keys in Each Node ===\n";
    Node::printAllKeys(n0);

    // 10. Adding New Node (100) to the Ring
    std::cout << "\n=== Adding Node(100) to the Ring ===\n";
    Node* n6 = new Node(100);
    n6->join(n0);

    // 11. Re-stabilize all nodes
    Node::stabilizeNetwork(n0);

    // 12. Run fix_fingers() again
    Node::fixAllFingers(n0);

    // 13. Print the updated ring structure
    std::cout << "\n=== Ring Structure After Adding Node(100) ===\n";
    printRing(n0);

    // // 14. Print each node’s updated finger table
    // std::cout << "\n=== Updated Finger Tables After Adding Node(100) ===\n";
    // Node::printAllFingerTables(n0);


    // 15. Print stored keys again (keys may be reassigned)
    std::cout << "\n=== Updated Stored Key-Value Pairs After Adding Node(100) ===\n";
    Node::printAllKeys(n0);

    // 19. Re-stabilize the network after removal
    Node::stabilizeNetwork(n0);

    // 20. Print the updated ring structure
    std::cout << "\n=== Ring Structure After Removing Node 65 ===\n";
    printRing(n0);
    
    // 20. Fix finger tables for all nodes
    Node::fixAllFingers(n0);
    std::cout << "\n=== Updated Finger Tables After Removing Node 65 ===\n";
    Node::printAllFingerTables(n0);


    // // 21. Print stored keys again to verify successful transfer
    // std::cout << "\n=== Updated Stored Key-Value Pairs After Removing Node 65 ===\n";
    // Node::printAllKeys(n0);

    // 8. Clean up
    Node::deleteAllNodes(n0);

    return 0;
}