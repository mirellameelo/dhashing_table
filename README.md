# Chord DHT Simulator

A peer-to-peer distributed lookup system simulator implementing the Chord protocol for efficient key-value storage and retrieval.

## 🚨 Note

This simulator is part of the Computer Networks course (CSE 250A) at UC Santa Cruz, 2025.

Student: Ella Ferraz.


## 🔧 Build and Run

1. Ensure you have g++ installed:

```bash
sudo apt install g++
```
2. Navigate to the project folder

```bash
cd dhashing_table
```
3. Compile the project:

```bash
make
```

4. Run

```bash
./dht_simulator
```

## 📝 Output Format

The simulator executes the following tasks sequentially:

### 1. Node Addition

- Creates and joins nodes into the Chord ring
- Initializes successor and predecessor relationships

### 2. Finger Table Display

- Shows finger table for each node
- Displays intervals and successors

### 3. Key Operations

- Key insertion and assignment
- Storage distribution visualization
- Dynamic key migration during node joins

### 4. Key Lookup Simulation

- Performs lookups from multiple nodes
- Displays node traversal paths

### 5. Node Removal Demonstration

- Handles node departure
- Shows key redistribution
- Updates routing tables