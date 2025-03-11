#include <iostream>
#include <cmath>
#include "node.h"

static const uint16_t RING_SIZE = (1 << BITLENGTH);

/**
 * @brief Helper function to print node info in the style you showed:
 *        -- Node id
 *           Successor / Predecessor
 *           Then each finger entry of the form:
 *              | k = i [start , end )  succ. = ??? |
 */
void printNodeInfo(Node* node) {
    if (!node) return;
    uint8_t thisId = node->getId();

    // Successor / Predecessor IDs
    Node* succ = node->getSuccessor();
    Node* pred = node->getPredecessor();
    int succId = succ ? succ->getId() : -1;  // or -1 if none
    int predId = pred ? pred->getId() : -1;  // or -1 if none

    // Print header
    std::cout << "-----------Node id:" << (int)thisId << "-----------\n";
    std::cout << "Successor: " << succId << "  Predecessor: " << predId << "\n";
    std::cout << "FingerTables:\n";

    // Each finger entry
    // The i-th finger covers the interval [start, end)
    // start = (id + 2^(i-1)) % 2^BITLENGTH
    // end   = (id + 2^i)     % 2^BITLENGTH
    for (int i = 1; i <= BITLENGTH; i++) {
        uint16_t start = (thisId + (1 << (i - 1))) % RING_SIZE;
        uint16_t end   = (thisId + (1 << i))       % RING_SIZE;
        Node* fingerSucc = node->getFinger(i);  // We’ll add getFinger() to Node
        int fingerId = fingerSucc ? fingerSucc->getId() : -1;

        std::cout << "| k = " << i
                  << " [ " << start << " , " << end << " )"
                  << "   succ. = " << fingerId << " |\n";
    }
    std::cout << "*************************\n\n";
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
    n0->join(nullptr);  // first node
    n1->join(n0);
    n2->join(n1);
    n3->join(n2);
    n4->join(n3);
    n5->join(n4);

    // 3. Stabilize/fix_fingers repeatedlyij
    for (int i = 0; i < 5; i++) {
        n0->stabilize();  n0->fix_fingers();
        n1->stabilize();  n1->fix_fingers();
        n2->stabilize();  n2->fix_fingers();
        n3->stabilize();  n3->fix_fingers();
        n4->stabilize();  n4->fix_fingers();
        n5->stabilize();  n5->fix_fingers();
    }

    // 4. Print each node's info in the requested format
    printNodeInfo(n0);
    printNodeInfo(n1);
    printNodeInfo(n2);
    printNodeInfo(n3);
    printNodeInfo(n4);
    printNodeInfo(n5);

    // 5. Clean up
    delete n0;
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;

    return 0;
}