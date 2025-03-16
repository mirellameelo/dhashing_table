#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "finger_table.h"

#define BITLENGTH 8  // Chord ring size is 2^BITLENGTH (Max nodes in the ring: 2^8 = 256)

/**
 * @class Node
 * @brief Represents a node in the Chord Distributed Hash Table (DHT) system.
 */
class Node {
public:
    /**
     * @brief Constructs a node with a given ID.
     * @param id Unique identifier of the node in the Chord ring.
     */
    Node(uint8_t id);

    /**
     * @brief Joins the Chord network.
     * @param knownNode An existing node in the network. Pass nullptr for the first node.
     */
    void join(Node* knownNode);

    /**
     * @brief Collects all nodes in the network starting from this node.
     * @return A vector containing all nodes in the Chord ring.
     */
    std::vector<Node*> collectAllNodes();

    /**
     * @brief Runs the stabilization protocol on all nodes.
     * @param nodes Vector of all nodes in the network.
     */
    static void stabilizeAll(std::vector<Node*>& nodes);

    /**
     * @brief Stabilizes the entire Chord network, ensuring successor and predecessor correctness.
     * @param startNode A node in the network to begin stabilization.
     */
    static void stabilizeNetwork(Node* startNode);

    /**
     * @brief Fixes the finger tables of all nodes.
     * @param startNode A node in the network to begin updating finger tables.
     */
    static void fixAllFingers(Node* startNode);

    /**
     * @brief Prints all stored key-value pairs in the Chord network.
     * @param startNode A node in the network to start collecting data.
     */
    static void printAllKeys(Node* startNode);

    /**
     * @brief Prints the finger tables of all nodes.
     * @param startNode A node in the network to begin printing.
     */
    static void printAllFingerTables(Node* startNode);

    /**
     * @brief Prints the Chord ring structure (successors & predecessors).
     * @param startNode A node in the network to start printing.
     */
    static void printRing(Node* startNode);

    /**
     * @brief Deletes all nodes from memory.
     * @param startNode A node in the network to begin deletion.
     */
    static void deleteAllNodes(Node* startNode);

    /**
     * @brief Inserts a key-value pair into the Chord ring.
     * @param key The key to store.
     * @param value The value associated with the key.
     */
    void insert(uint8_t key, int value);

    /**
     * @brief Inserts a key with a default "None" value (-1).
     * @param key The key to store.
     */
    void insert(uint8_t key);

    /**
     * @brief Removes this node from the Chord network and migrates its keys.
     */
    void leave();

    /**
     * @brief Removes a specific key from the Chord ring.
     * @param key The key to be removed.
     */
    void removeKey(uint8_t key);

    /**
     * @brief Finds the successor node responsible for a given key.
     * @param key The key to find.
     * @return The successor node responsible for the key.
     */
    Node* find_successor(uint8_t key);

    /**
     * @brief Prints the node's finger table.
     */
    void print_finger_table();

    /**
     * @brief Prints the keys stored locally on this node.
     */
    void print_keys();

    /**
     * @brief Runs the stabilization protocol on this node.
     */
    void stabilize();

    /**
     * @brief Notifies this node about potential predecessor changes.
     * @param n The notifying node.
     */
    void notify(Node* n);

    /**
     * @brief Periodically updates this node’s finger table.
     */
    void fix_fingers();

    /**
     * @brief Finds the node responsible for a given key and prints the lookup path.
     * @param key The key to find.
     */
    void find(uint8_t key);

    /**
     * @brief Gets the ID of this node.
     * @return The node's ID.
     */
    uint8_t getId();

    /**
     * @brief Gets the successor of this node.
     * @return Pointer to the successor node.
     */
    Node* getSuccessor() { return successor_; }

    /**
     * @brief Gets the predecessor of this node.
     * @return Pointer to the predecessor node.
     */
    Node* getPredecessor() { return predecessor_; }

    /**
     * @brief Gets the i-th entry in the finger table.
     * @param i The index in the finger table.
     * @return Pointer to the finger node at index i.
     */
    Node* getFinger(int i) { return fingerTable_.get(i); }

    /**
     * @brief Sets the successor node.
     * @param node Pointer to the new successor.
     */
    void setSuccessor(Node* node);

    /**
     * @brief Sets the predecessor node.
     * @param node Pointer to the new predecessor.
     */
    void setPredecessor(Node* node);

private:
    uint8_t id_;                     ///< Unique node ID in [0 .. 2^BITLENGTH - 1]
    FingerTable fingerTable_;         ///< Finger table for efficient lookups
    std::map<uint8_t, int> localKeys_; ///< Locally stored key-value pairs
    Node* successor_;                ///< Pointer to this node’s successor
    Node* predecessor_;              ///< Pointer to this node’s predecessor
    size_t nextFingerToFix_;         ///< Used for periodic finger table maintenance

    /**
     * @brief Finds the closest preceding finger for a given key.
     * @param key The key being searched for.
     * @return Pointer to the closest preceding finger.
     */
    Node* closest_preceding_finger(uint8_t key);

    /**
     * @brief Checks if a value is in a given ring interval.
     * @param x The value to check.
     * @param start The interval start.
     * @param end The interval end.
     * @param inclusiveStart Whether to include the start in the interval.
     * @param inclusiveEnd Whether to include the end in the interval.
     * @return True if x is in the interval, false otherwise.
     */
    bool inInterval(uint8_t x, uint8_t start, uint8_t end,
                    bool inclusiveStart = false, bool inclusiveEnd = false);
};

#endif  // NODE_H
