#include "node.h"
#include <iostream>
#include <limits>
#include <cmath>

// --------------------
// Node Implementation
// --------------------
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
    }

    fingerTable_.initialize();
}


// Find successor
Node* Node::find_successor(uint8_t key) {
    if (inInterval(key, id_, successor_->getId(), false, true)) {
        return successor_;
    } else {
        Node* node = closest_preceding_finger(key);
        return node == this ? successor_ : node->find_successor(key);
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
void Node::insert(uint8_t key) {
    Node* responsible = find_successor(key);
    responsible->localKeys_[key] = key;
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
    std::cout << "Keys at Node " << (int)id_ << ": ";
    if (localKeys_.empty()) {
        std::cout << "(none)";
    } else {
        for (auto& kv : localKeys_) {
            std::cout << (int)kv.first << " ";
        }
    }
    std::cout << "\n";
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

    // ✅ Ensure predecessor is correctly set
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

