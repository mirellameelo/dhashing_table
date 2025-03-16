# Chord DHT Simulator

A peer-to-peer distributed lookup system simulator implementing the Chord protocol for efficient key-value storage and retrieval.

## 🔧 Build

1. Ensure you have g++ installed:

```bash
sudo apt install g++
```

1. Compile the project:

```bash
make
```

Run:

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