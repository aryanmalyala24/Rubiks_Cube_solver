#ifndef STATE_DATABASE_H
#define STATE_DATABASE_H

#include "../cube/Cube.h"
#include "../cube/Moves.h"

#include <vector>
#include <cstdint>
#include <string>

using StateKey = uint64_t;

struct DBEntry {
    uint32_t pIdx;
    Move m;
};

// Flat array slot for Open Addressing Linear Probing
struct MapSlot {
    uint64_t k;
    uint32_t v;
};

class StateDatabase {
public:
    void build(int md);
    bool contains(const Cube& cb) const;
    std::vector<Move> getSolution(const Cube& cb) const;
    size_t size() const;
    bool save(const string& fn) const;
    bool load(const string& fn);
    int getDistance(const Cube& cb) const;

private:
    StateKey getKey(const Cube& cb) const;
    
    std::vector<MapSlot> table;
    uint64_t mask;
    size_t cnt;

    void initTable(size_t cap);
    void insertMap(uint64_t k, uint32_t v);
    uint32_t findMap(uint64_t k) const;

    std::vector<DBEntry> entries;
    std::vector<StateKey> keys;
};

#endif