#ifndef FINGER_TABLE_H
#define FINGER_TABLE_H

#include <vector>

// Forward declare Node to avoid circular dependency
class Node;

class FingerTable {
public:
    explicit FingerTable(Node* owner);

    /**
     * @brief Get the finger entry at index i.
     */
    Node* get(int i) const;

    /**
     * @brief Set the finger entry at index i.
     */
    void set(int i, Node* node);

    /**
     * @brief Initialize finger table based on the node's position.
     */
    void initialize();

    /**
     * @brief Print the finger table for debugging.
     */
    void prettyPrint();

private:
    Node* owner_;                   // The node that owns this finger table
    std::vector<Node*> fingers_;    // Stores pointers to nodes for routing
};

#endif  // FINGER_TABLE_H
