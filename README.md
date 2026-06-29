Compile instructions:
g++ -std=c++17 -Ofast -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -DNDEBUG src/main.cpp src/cube/Cube.cpp src/cube/Moves.cpp src/database/StateDatabase.cpp src/search/DFS.cpp src/search/Heuristic.cpp src/search/IDAStar.cpp src/search/IDDFS.cpp src/search/MoveGenerator.cpp src/search/Node.cpp src/solver/Solver.cpp src/utils/Scrambler.cpp -o benchmark
.\benchmark  

# Rubik's Cube Solver

A high-performance Rubik's Cube solver written in **C++17**, featuring an optimized **IDA*** search engine accelerated by a **depth-7 reverse state database** containing over **109 million cube states**.

The project focuses on algorithmic search, heuristic optimization, cache-conscious implementation, and performance engineering rather than relying on external solving libraries.

---

## Features

* Full 3×3×3 Rubik's Cube simulation
* Accurate implementation of all 18 legal cube moves
* Random scramble generation
* Multiple search algorithms

  * Depth First Search (DFS)
  * Iterative Deepening DFS (IDDFS)
  * Iterative Deepening A* (IDA*)
* Reverse Breadth-First Search database generation
* Database-assisted admissible heuristic
* Automatic benchmarking framework
* Performance statistics

  * Solve time
  * Nodes expanded
  * Solution length
  * Hardest scramble detection
* Modern C++17 implementation
* Optimized for high search performance

---

## Project Structure

```
RubiksCubeSolver
│
├── docs/
│   └── benchmarks.md
│
├── src/
│   ├── cube/
│   │   ├── Cube.cpp
│   │   ├── Cube.h
│   │   ├── Moves.cpp
│   │   └── Moves.h
│   │
│   ├── database/
│   │   ├── StateDatabase.cpp
│   │   └── StateDatabase.h
│   │
│   ├── search/
│   │   ├── DFS.cpp
│   │   ├── IDDFS.cpp
│   │   ├── IDAStar.cpp
│   │   ├── Heuristic.cpp
│   │   ├── MoveGenerator.cpp
│   │   ├── Node.cpp
│   │   └── SearchStats.h
│   │
│   ├── solver/
│   │   ├── Solver.cpp
│   │   └── Solver.h
│   │
│   ├── utils/
│   │   ├── Scrambler.cpp
│   │   └── Scrambler.h
│   │
│   └── main.cpp
│
├── CMakeLists.txt
└── README.md
```

---

# Search Pipeline

```
Random Scramble
        │
        ▼
Cube Representation
        │
        ▼
IDA* Search
        │
        ▼
Database Heuristic
        │
        ▼
Goal State
        │
        ▼
Solution Moves
```

---

# Database

The solver uses a reverse Breadth-First Search to generate a state database from the solved cube.

Current database:

* Depth: **7**
* States Stored: **109,541,763**
* Approximate Size: **1.63 GB**

The database stores the minimum distance from every recorded state to the solved cube.

During solving, IDA* searches only until a database state is reached, after which the stored distance provides an admissible heuristic.

---

# Algorithms Used

## Cube Representation

* 54-sticker representation
* Constant-time move application
* In-place state mutation

---

## Search

### DFS

Used for experimentation and validation.

### IDDFS

Used for comparison against heuristic search.

### IDA*

Primary solving algorithm.

Advantages:

* Low memory usage
* Optimal solutions
* Effective heuristic pruning
* Suitable for exponential search spaces

---

## Heuristic

Database-assisted admissible heuristic.

```
f(n) = g(n) + h(n)
```

where

* g(n) = search depth
* h(n) = stored database distance

This guarantees optimal solutions while dramatically reducing the search space.

---

# Benchmarking

The benchmark framework automatically

* Generates random scrambles
* Solves each scramble
* Records solve time
* Records expanded nodes
* Records solution length
* Detects hardest scramble
* Appends results to

```
docs/benchmarks.md
```

---

# Performance

Compiled using

```
g++ -std=c++17 \
-Ofast \
-march=native \
-mtune=native \
-flto \
-funroll-loops \
-fomit-frame-pointer \
-DNDEBUG
```

Example benchmark performance

| Scramble | Average Solve Time |
| -------- | -----------------: |
| 11 Moves |             ~32 ms |
| 12 Moves |            ~191 ms |
| 13 Moves |             ~2.4 s |
| 14 Moves |            ~20.8 s |

Performance depends heavily on scramble difficulty.

---

# Building

```
g++ -std=c++17 \
-Ofast \
-march=native \
-mtune=native \
-flto \
-funroll-loops \
-fomit-frame-pointer \
-DNDEBUG \
src/main.cpp \
src/cube/Cube.cpp \
src/cube/Moves.cpp \
src/database/StateDatabase.cpp \
src/search/DFS.cpp \
src/search/Heuristic.cpp \
src/search/IDAStar.cpp \
src/search/IDDFS.cpp \
src/search/MoveGenerator.cpp \
src/search/Node.cpp \
src/solver/Solver.cpp \
src/utils/Scrambler.cpp \
-o benchmark
```

Run

```
./benchmark
```

or on Windows

```
benchmark.exe
```

---

# Technologies

* C++17
* STL
* CMake
* Git
* GitHub

---

# Key Concepts

* Graph Search
* IDA*
* BFS
* DFS
* IDDFS
* Heuristic Search
* State Encoding
* Hashing
* Search Tree Pruning
* Performance Optimization
* Compiler Optimization
* Cache Efficiency

---

# Future Work

* Pattern Databases
* Parallel Search
* SIMD Move Generation
* Custom Flat Hash Table
* GPU Database Generation
* Kociemba Algorithm Integration
* Interactive GUI Visualizer
