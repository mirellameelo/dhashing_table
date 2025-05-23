#include "finger_table.h"
#include "node.h"
#include <iostream>

/**
 * @brief Constructor: Initializes an empty finger table.
 */
FingerTable::FingerTable(Node* owner) : owner_(owner), fingers_(BITLENGTH + 1, nullptr) {}

/**
 * @brief Get the node at index i in the finger table.
 */
Node* FingerTable::get(int i) const {
    if (i >= 1 && i <= BITLENGTH) {
        return fingers_[i];
    }
    return nullptr;
}

/**
 * @brief Set a node at index i in the finger table.
 */
void FingerTable::set(int i, Node* node) {
    if (i >= 1 && i <= BITLENGTH) {
        fingers_[i] = node;
    }
}

/**
 * @brief Initialize the finger table entries based on the node's ID.
 */
void FingerTable::initialize() {
    for (int i = 1; i <= BITLENGTH; i++) {
        uint16_t start = (owner_->getId() + (1 << (i - 1))) % (1 << BITLENGTH);
        fingers_[i] = owner_->find_successor(start);
    }
}

/**
 * @brief Pretty print the finger table.
 */
void FingerTable::prettyPrint() {
    std::cout << "------------------------------\n";
    std::cout << "Finger Table of Node " << (int)owner_->getId() << ":\n";
    std::cout << "  (Each entry k = i is calculated as: start = (ID + 2^(i-1)) mod " 
              << (1 << BITLENGTH) << ")\n";
    for (size_t i = 1; i <= BITLENGTH; ++i) {
        uint16_t start = (owner_->getId() + (1 << (i - 1))) % (1 << BITLENGTH);
        if (fingers_[i]) {
            std::cout << "  k = " << i << " (start = " << start << ") : Node "
                      << (int)fingers_[i]->getId() << "\n";
        } else {
            std::cout << "  k = " << i << " (start = " << start << ") : None\n";
        }
    }
    std::cout << "------------------------------\n";
}
