#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "finger_table.h"

#define BITLENGTH 8  // Chord ring size is 2^BITLENGTH

class Node;

class Node {
public:
    Node(uint8_t id);
    void join(Node* knownNode);
    static void stabilizeAll(std::vector<Node*>& nodes);
    std::vector<Node*> collectAllNodes();
    static void stabilizeNetwork(Node* startNode);
    static void fixAllFingers(Node* startNode);
    static void printAllKeys(Node* startNode);
    static void printAllFingerTables(Node* startNode);
    static void printRing(Node* startNode);
    static void deleteAllNodes(Node* startNode);
    Node* find(uint8_t key);
    void insert(uint8_t key, int value);
    void insert(uint8_t key);
    void leave();
    void removeKey(uint8_t key);
    Node* find_successor(uint8_t key);
    void print_finger_table();
    void print_keys();
    void stabilize();
    void notify(Node* n);
    void fix_fingers();

    uint8_t getId();
    Node* getSuccessor() { return successor_; }
    Node* getPredecessor() { return predecessor_; }
    Node* getFinger(int i) { return fingerTable_.get(i); }

    void setSuccessor(Node* node);
    void setPredecessor(Node* node);

private:
    uint8_t id_;                     // Unique node ID in [0 .. 2^BITLENGTH-1]
    FingerTable fingerTable_;         
    std::map<uint8_t, int> localKeys_; 
    Node* successor_;
    Node* predecessor_;
    size_t nextFingerToFix_;

    Node* closest_preceding_finger(uint8_t key);
    bool inInterval(uint8_t x, uint8_t start, uint8_t end,
                    bool inclusiveStart = false, bool inclusiveEnd = false);
};

#endif  // NODE_H
