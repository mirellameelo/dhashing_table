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

int main() {
    // 1. Create the nodes.
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

    // 3. Print ring structure
    std::cout << "\n=== Ring Structure After Joining ===\n";
    printRing(n0);

    // Clean up
    delete n0;
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;

    return 0;
}

