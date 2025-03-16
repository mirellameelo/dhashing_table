#include <iostream>
#include <cmath>
#include "node.h"

static const uint16_t RING_SIZE = (1 << BITLENGTH);

int main() {

    std::cout << "\n========================= Task 1: Add nodes =========================\n";
    // Create nodes
    Node* n0 = new Node(0);
    Node* n1 = new Node(30);
    Node* n2 = new Node(65);
    Node* n3 = new Node(110);
    Node* n4 = new Node(160);
    Node* n5 = new Node(230);

    // Join nodes
    n0->join(nullptr);
    n1->join(n0);
    n2->join(n1);
    n3->join(n2);
    n4->join(n3);
    n5->join(n4);

    // Stabilize the network and finger tables
    Node::stabilizeNetwork(n0);
    Node::fixAllFingers(n0);

    // Print the ring structure and finger tables
    std::cout << "\n========================= Task 2: Print finger table of all nodes =========================\n";
    Node::printAllFingerTables(n0);

   // Insert keys into the Chord ring
   std::cout << "\n========================= Task 3: Inserting Keys =========================\n";
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

   // Print all stored keys
   std::cout << "\n========================= Task 3.1: Print keys in each node =========================\n";
   Node::printAllKeys(n0);

   // Adding a new node (100)
   std::cout << "\n========================= Task 3.2: Adding Node (100), and printing migrated keys =========================\n";
   Node* n6 = new Node(100);
   n6->join(n0);
   Node::stabilizeNetwork(n0);
   Node::fixAllFingers(n0);
   
   std::vector<uint8_t> keysToLookup = {3, 200, 123, 45, 99, 60, 50, 100, 101, 102, 240, 250};
   std::vector<Node*> lookupNodes = {n0, n2, n6};
   
   std::cout << "\n========================= Task 4: Lookup keys from specific nodes =========================\n";
   for (Node* lookupNode : lookupNodes) {
       for (uint8_t key : keysToLookup) {
           lookupNode->find(key);
        }
        std::cout << "--------------------------------\n";
    }

    // Print the ring structure and finger tables after removal
    std::cout << "\n========================= Task 5: Removing Node 65, and printing finger table =========================\n";
    n2->leave();
    Node::stabilizeNetwork(n0);
    Node::printAllFingerTables(n0);

    // Clean up
    std::cout << "\n========================= Cleaning Up: Deleting All Nodes =========================\n";
    Node::deleteAllNodes(n0);

    return 0;
}