#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "finger_table.h"

#define BITLENGTH 8  // Chord ring size is 2^BITLENGTH

class Node;  // Forward declaration

// --------------------
// Node Class (Chord Node)
// --------------------
class Node {
public:
    Node(uint8_t id);

    /**
     * @brief Join the Chord network
     * @param knownNode: An existing node in the network. If this is
     *        the first node in the network, pass nullptr.
     */
    void join(Node* knownNode);
    static void stabilizeAll(std::vector<Node*>& nodes);
    std::vector<Node*> collectAllNodes();
    static void stabilizeNetwork(Node* startNode);


    /**
     * @brief Insert a key into the DHT.
     * @param key: The key to be stored.
     */
    void insert(uint8_t key, int value);
    void insert(uint8_t key);
    void leave();

    /**
     * @brief Remove a key from the DHT.
     * @param key: The key to be deleted.
     */
    void removeKey(uint8_t key);

    /**
     * @brief Find the node responsible for 'key' (returns the Node pointer).
     */
    Node* find_successor(uint8_t key);

    /**
     * @brief Print the node's finger table.
     */
    void print_finger_table();

    /**
     * @brief Print stored keys.
     */
    void print_keys();

    /**
     * @brief Periodic procedures described in the Chord paper
     */
    void stabilize();
    void notify(Node* n);
    void fix_fingers();

    /**
     * @return The ID of this node.
     */
    uint8_t getId();
    Node* getSuccessor() { return successor_; }
    Node* getPredecessor() { return predecessor_; }
    Node* getFinger(int i) { return fingerTable_.get(i); }

    /**
     * @brief Set successor and predecessor nodes
     */
    void setSuccessor(Node* node);
    void setPredecessor(Node* node);

private:
    uint8_t id_;                     // Unique node ID in [0 .. 2^BITLENGTH-1]
    FingerTable fingerTable_;         // ✅ Remove duplicate!
    std::map<uint8_t, int> localKeys_;  // Keys this node is responsible for
    Node* successor_;                // Pointer to successor node
    Node* predecessor_;              // Pointer to predecessor node
    size_t nextFingerToFix_;         // Used by fix_fingers()

    /**
     * @brief Return the closest finger preceding 'key'.
     */
    Node* closest_preceding_finger(uint8_t key);

    /**
     * @brief Simple ring interval check (handles wrap-around).
     */
    bool inInterval(uint8_t x, uint8_t start, uint8_t end,
                    bool inclusiveStart = false, bool inclusiveEnd = false);
};

#endif  // NODE_H
