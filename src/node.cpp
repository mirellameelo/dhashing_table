#include "node.h"
#include <iostream>
#include <limits>
#include <cmath>

// --------------------
// Finger Table
// --------------------
FingerTable::FingerTable(uint8_t nodeId) : nodeId_(nodeId) {
    // Index 0 is unused; entries go from 1..BITLENGTH
    fingerTable_.resize(BITLENGTH + 1, nullptr);
}

void FingerTable::set(size_t index, Node* successor) {
    if (index < fingerTable_.size()) {
        fingerTable_[index] = successor;
    }
}

Node* FingerTable::get(size_t index) {
    if (index < fingerTable_.size()) {
        return fingerTable_[index];
    }
    return nullptr;
}

void FingerTable::prettyPrint() {
    std::cout << "Finger Table of Node " << (int)nodeId_ << ":\n";
    for (size_t i = 1; i <= BITLENGTH; ++i) {
        if (fingerTable_[i]) {
            std::cout << "  Entry " << i << ": Node "
                      << (int)fingerTable_[i]->getId() << "\n";
        } else {
            std::cout << "  Entry " << i << ": None\n";
        }
    }
}

// --------------------
// Node
// --------------------
Node::Node(uint8_t id)
    : id_(id),
      fingerTable_(id),
      successor_(this), // Initially points to itself (single-node ring)
      predecessor_(nullptr),
      nextFingerToFix_(1)
{
}

// Helper: ring interval check
bool Node::inInterval(uint8_t x, uint8_t start, uint8_t end,
                      bool inclusiveStart, bool inclusiveEnd) {
    // We interpret [start, end) in the modulo 2^BITLENGTH sense.
    // If start <= end: check x in (start, end) with possible inclusivity
    // If start > end: the interval wraps around.
    const uint16_t ringSize = (1 << BITLENGTH);

    // A small lambda to shift everything so 'start' is treated as 0
    auto modDist = [&](uint16_t a, uint16_t b) {
        return (a + ringSize - b) % ringSize;
    };

    if (start == end) {
        // This can represent the entire ring if inclusive or an empty interval if exclusive.
        if (inclusiveStart || inclusiveEnd) {
            return true;  // entire ring
        }
        return false;
    }

    bool inRange = false;
    uint16_t shiftedX   = modDist(x, start);
    uint16_t shiftedEnd = modDist(end, start);

    // Check boundary cases for exact equality with start or end
    if (x == start && inclusiveStart) {
        return true;
    }
    if (x == end && inclusiveEnd) {
        return true;
    }

    // If shiftedEnd == 0, that means 'end' == 'start' in mod sense => entire ring
    if (shiftedEnd == 0) {
        // entire ring
        inRange = true;
    } else {
        // We want 0 < shiftedX < shiftedEnd if we do not count boundaries.
        if (shiftedX > 0 && shiftedX < shiftedEnd) {
            inRange = true;
        }
    }
    return inRange;
}

// Return this node's ID
uint8_t Node::getId() {
    return id_;
}

// Join
void Node::join(Node* knownNode) {
    if (knownNode == nullptr) {
        // First node in the network
        predecessor_ = nullptr;
        successor_ = this;
        std::cout << "Node " << (int)id_ 
                  << " created as FIRST node in Chord.\n";
    } else {
        // Use knownNode to find my successor
        successor_ = knownNode->find_successor(id_);
        predecessor_ = nullptr;
        std::cout << "Node " << (int)id_
                  << " joined via Node " << (int)knownNode->getId() << "\n";
    }
}

// Find successor
Node* Node::find_successor(uint8_t key) {
    // Check if key is in (this->id, successor_->id] (mod 2^BITLENGTH)
    if (inInterval(key, id_, successor_->getId(), false, true)) {
        return successor_;
    } else {
        // Forward to the closest finger
        Node* node = closest_preceding_finger(key);
        if (node == this) {
            // Fallback: can't go further, just return our successor
            return successor_;
        }
        return node->find_successor(key);
    }
}

// The missing method that looks through our finger table from highest to lowest
Node* Node::closest_preceding_finger(uint8_t key) {
    // Search from BITLENGTH down to 1
    for (int i = BITLENGTH; i >= 1; i--) {
        Node* f = fingerTable_.get(i);
        if (f && f != this) {
            // If f is in (this->id, key) then return f
            if (inInterval(f->getId(), id_, key, false, false)) {
                return f;
            }
        }
    }
    return this; // If none found, return this node
}

// Insert a key
void Node::insert(uint8_t key) {
    Node* responsible = find_successor(key);
    // Just store the key in a map (or any container). 
    // We'll store key->key for demonstration.
    responsible->localKeys_[key] = key;
}

// Remove a key
void Node::removeKey(uint8_t key) {
    Node* responsible = find_successor(key);
    auto it = responsible->localKeys_.find(key);
    if (it != responsible->localKeys_.end()) {
        responsible->localKeys_.erase(it);
    }
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
    if (successor_ == this) {
        // Single-node ring: nothing to do
        return;
    }
    // Check my successor's predecessor
    Node* x = successor_->predecessor_;
    if (x != nullptr && x != successor_ &&
        inInterval(x->getId(), id_, successor_->getId(), false, false))
    {
        successor_ = x;  // There's a closer node
    }
    successor_->notify(this);
}

// notify
void Node::notify(Node* n) {
    if (predecessor_ == nullptr) {
        predecessor_ = n;
    } else if (inInterval(n->getId(), predecessor_->getId(), id_, false, false)) {
        predecessor_ = n;
    }
}

// fix_fingers
void Node::fix_fingers() {
    // The i-th finger starts at (id + 2^(i-1)) mod (2^BITLENGTH)
    uint16_t start = (id_ + (1 << (nextFingerToFix_ - 1))) % (1 << BITLENGTH);
    Node* succ = find_successor((uint8_t)start);
    fingerTable_.set(nextFingerToFix_, succ);

    nextFingerToFix_ = nextFingerToFix_ + 1;
    if (nextFingerToFix_ > BITLENGTH) {
        nextFingerToFix_ = 1;
    }
}
