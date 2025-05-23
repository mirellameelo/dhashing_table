#include "node.h"
#include <iostream>
#include <limits>
#include <cmath>

Node::Node(uint8_t id)
    : id_(id),
      fingerTable_(this),
      successor_(this),
      predecessor_(nullptr),
      nextFingerToFix_(1)
{
}

// Helper: ring interval check
bool Node::inInterval(uint8_t x, uint8_t start, uint8_t end,
                      bool inclusiveStart, bool inclusiveEnd) {
    const uint16_t ringSize = (1 << BITLENGTH);

    // A lambda to normalize distances in the ring
    auto modDist = [&](uint16_t a, uint16_t b) {
        return (a + ringSize - b) % ringSize;
    };

    if (start == end) {
        return inclusiveStart || inclusiveEnd;
    }

    uint16_t shiftedX   = modDist(x, start);
    uint16_t shiftedEnd = modDist(end, start);

    if (x == start && inclusiveStart) return true;
    if (x == end && inclusiveEnd) return true;
    if (shiftedEnd == 0) return true;  // Whole ring case
    return (shiftedX > 0 && shiftedX < shiftedEnd);
}

// Return this node's ID
uint8_t Node::getId() {
    return id_;
}

// Join
void Node::join(Node* knownNode) {
    if (knownNode == nullptr) {
        predecessor_ = nullptr;
        successor_ = this;
        std::cout << "Node " << (int)id_ << " created as FIRST node in Chord.\n";
    } else {
        successor_ = knownNode->find_successor(id_);
        predecessor_ = successor_->getPredecessor();

        if (!predecessor_ || predecessor_ == successor_) {
            predecessor_ = knownNode;
        }

        successor_->setPredecessor(this);
        predecessor_->setSuccessor(this);

        std::cout << "Node " << (int)id_
                  << " joined via Node " << (int)knownNode->getId() << "\n";

        std::vector<uint8_t> keysToMigrate;
        for (auto& kv : successor_->localKeys_) {
            if (inInterval(kv.first, predecessor_->getId(), id_, false, true)) {
                keysToMigrate.push_back(kv.first);
            }
        }

        for (uint8_t key : keysToMigrate) {
            localKeys_[key] = successor_->localKeys_[key]; // Move key-value pair
            successor_->localKeys_.erase(key); // Remove from old node
            std::cout << "Migrated key " << (int)key << " to Node " << (int)id_ << std::endl;
        }
    }

    fingerTable_.initialize();
}

void Node::stabilizeAll(std::vector<Node*>& nodes) {
    for (int i = 0; i < 5; i++) {  // Run multiple rounds for full propagation
        for (Node* node : nodes) {
            node->stabilize();
        }
    }
}

void Node::fixAllFingers(Node* startNode) {
    // Collect all nodes dynamically
    std::vector<Node*> allNodes = startNode->collectAllNodes();

    // Run fix_fingers on each node multiple times
    //std::cout << "\n=== Fixing Finger Tables for All Nodes ===\n";
    for (int i = 0; i < 5; i++) {  // Run multiple times for full updates
        for (Node* node : allNodes) {
            node->fix_fingers();
        }
    }
}

void Node::printAllKeys(Node* startNode) {
    // Collect all nodes dynamically
    std::vector<Node*> allNodes = startNode->collectAllNodes();

    // Print keys for each node
    //std::cout << "\n=== Stored Key-Value Pairs in All Nodes ===\n";
    for (Node* node : allNodes) {
        node->print_keys();
    }
}

void Node::printAllFingerTables(Node* startNode) {
    // Collect all nodes dynamically
    std::vector<Node*> allNodes = startNode->collectAllNodes();

    // Print finger tables for each node
    //std::cout << "\n=== Finger Tables of All Nodes ===\n";
    for (Node* node : allNodes) {
        node->print_finger_table();
    }
}

void Node::printRing(Node* startNode) {
    if (!startNode) return;
    
    std::cout << "\n=== Chord Ring Structure ===\n";

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

void Node::find(uint8_t key) {
    std::cout << "\n Look-up result of key " << (int)key
              << " from Node " << (int)this->getId() << ":\n";

    Node* responsibleNode = this->find_successor(key);
    int value = -1;

    if (responsibleNode->localKeys_.count(key)) {
        value = responsibleNode->localKeys_[key];
    }

    std::cout << " Found at Node " << (int)responsibleNode->getId() << "\n"
              << " Key " << (int)key << " -> Value: "
              << (value == -1 ? "None" : std::to_string(value)) << "\n";
}

void Node::deleteAllNodes(Node* startNode) {
    // Collect all nodes dynamically
    std::vector<Node*> allNodes = startNode->collectAllNodes();

    // Delete all nodes
    //std::cout << "\n=== Cleaning Up: Deleting All Nodes ===\n";
    for (Node* node : allNodes) {
        delete node;
    }
}

std::vector<Node*> Node::collectAllNodes() {
    std::vector<Node*> nodes;
    Node* current = this;

    do {
        nodes.push_back(current);
        current = current->getSuccessor();
    } while (current != this);  // Stop when we complete the ring

    return nodes;
}

void Node::stabilizeNetwork(Node* startNode) {
    // Collect all nodes dynamically from the given starting node
    std::vector<Node*> allNodes = startNode->collectAllNodes();

    // Run stabilization on all collected nodes
    //std::cout << "\n=== Stabilizing all nodes ===\n";
    Node::stabilizeAll(allNodes);
}

void Node::leave() {
    std::cout << "Node " << (int)id_ << " is leaving the ring.\n";

    if (successor_ == this && predecessor_ == nullptr) {
        // Only one node in the ring, it can simply leave.
        std::cout << "Last node in the ring. Removing it.\n";
        return;
    }

    if (successor_ != this) {
        for (auto& kv : localKeys_) {
            successor_->localKeys_[kv.first] = kv.second;
            std::cout << "Transferred key " << (int)kv.first << " to Node " << (int)successor_->getId() << "\n";
        }
        localKeys_.clear();  // Empty key storage from this node
    }

    if (predecessor_) {
        predecessor_->setSuccessor(successor_);
    }
    if (successor_) {
        successor_->setPredecessor(predecessor_);
    }

    std::cout << "Node " << (int)id_ << " has left the ring.\n";
}

// Find successor
Node* Node::find_successor(uint8_t key) {
    // If the key is exactly this node's ID, we are responsible.
    if (key == id_) {
        return this;
    }

    // Otherwise check if key is in (id_, successor->id].
    if (inInterval(key, id_, successor_->getId(), false, true)) {
        return successor_;
    } else {
        Node* node = closest_preceding_finger(key);
        // If node == this, we can just return successor_, or handle it carefully:
        return (node == this) ? successor_ : node->find_successor(key);
    }
}

void Node::setSuccessor(Node* node) {
    successor_ = node;
}

void Node::setPredecessor(Node* node) {
    predecessor_ = node;
}


// Find the closest preceding finger for a given key
Node* Node::closest_preceding_finger(uint8_t key) {
    for (int i = BITLENGTH; i >= 1; i--) {
        Node* f = fingerTable_.get(i);
        if (f && f != this && inInterval(f->getId(), id_, key, false, false)) {
            return f;
        }
    }
    return this;
}

// Insert a key
// Overloaded insert() - Default to "None" when value is not provided
void Node::insert(uint8_t key) {
    insert(key, -1);  // Call the other insert with a default "None" value
}

// Main insert function - Stores key-value pairs
void Node::insert(uint8_t key, int value) {
    Node* responsible = find_successor(key);
    
    // Store key with either an actual value or mark it as None (-1)
    responsible->localKeys_[key] = value;

    std::cout << "Key " << (int)key << " stored at Node " << (int)responsible->getId()
              << " with value " << (value == -1 ? "None" : std::to_string(value)) << std::endl;
}

// Remove a key
void Node::removeKey(uint8_t key) {
    Node* responsible = find_successor(key);
    responsible->localKeys_.erase(key);
}

// Print finger table
void Node::print_finger_table() {
    fingerTable_.prettyPrint();
}

// Print the keys stored locally on this node
void Node::print_keys() {
    std::cout << "Node id:" << (int)id_ << "\n";
    if (localKeys_.empty()) {
        std::cout << "(No keys stored)\n";
    } else {
        std::cout << "{ ";
        for (auto& kv : localKeys_) {
            std::cout << (int)kv.first << ": " 
                      << (kv.second == -1 ? "None" : std::to_string(kv.second)) << ", ";
        }
        std::cout << "}\n";
    }
}

// Periodic stabilize
void Node::stabilize() {
    if (successor_ == this) return;

    Node* x = successor_->getPredecessor();
    if (x != nullptr && x != successor_ &&
        inInterval(x->getId(), id_, successor_->getId(), false, false))
    {
        successor_ = x;
    }

    if (successor_->getPredecessor() == nullptr || 
        inInterval(successor_->getPredecessor()->getId(), id_, successor_->getId(), false, false)) {
        successor_->setPredecessor(this);
    }

    successor_->notify(this);
}

// notify
void Node::notify(Node* n) {
    if (predecessor_ == nullptr || inInterval(n->getId(), predecessor_->getId(), id_, false, false)) {
        predecessor_ = n;
    }
}

// fix_fingers
void Node::fix_fingers() {
    for (int i = 1; i <= BITLENGTH; i++) {
        uint16_t start = (id_ + (1 << (i - 1))) % (1 << BITLENGTH);
        Node* succ = find_successor(start);

        fingerTable_.set(i, succ);
    }
}

