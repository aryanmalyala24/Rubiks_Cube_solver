#include "StateDatabase.h"
#include "../search/MoveGenerator.h"
#include "../search/Node.h"
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;

struct BFSNode {
    Cube cb;
    int d;
    Move lm;
};

StateKey StateDatabase::getKey(const Cube& cb) const {
    return cb.hash;
}

void StateDatabase::initTable(size_t c) {
    // Calculates the next power of 2 that doubles the target capacity
    // For 115 million states, this creates an array of ~268 million slots, 
    // ensuring the load factor never breaches ~42%, keeping lookups at O(1).
    size_t cap = 1;
    while(cap < c * 2) cap *= 2; 
    table.assign(cap, {0, 0xFFFFFFFF});
    mask = cap - 1;
    cnt = 0;
}

void StateDatabase::insertMap(uint64_t k, uint32_t v) {
    size_t i = k & mask;
    while (table[i].v != 0xFFFFFFFF && table[i].k != k) {
        i = (i + 1) & mask;
    }
    if (table[i].v == 0xFFFFFFFF) {
        table[i].k = k;
        table[i].v = v;
        cnt++;
    }
}

uint32_t StateDatabase::findMap(uint64_t k) const {
    if (table.empty()) return 0xFFFFFFFF;
    size_t i = k & mask;
    while (table[i].v != 0xFFFFFFFF) {
        if (table[i].k == k) return table[i].v;
        i = (i + 1) & mask;
    }
    return 0xFFFFFFFF; // 0xFFFFFFFF acts as a null/empty sentinel 
}

size_t StateDatabase::size() const { return cnt; }

bool StateDatabase::contains(const Cube& cb) const { 
    return findMap(getKey(cb)) != 0xFFFFFFFF; 
}

vector<Move> StateDatabase::getSolution(const Cube& cb) const {
    vector<Move> s;
    uint32_t idx = findMap(getKey(cb));
    if(idx == 0xFFFFFFFF) return s;
    while(idx != 0) {
        const DBEntry& e = entries[idx];
        s.push_back(e.m);
        idx = e.pIdx;
    }
    return s;
}

void StateDatabase::build(int md) {
    table.clear(); entries.clear(); keys.clear();
    size_t cap = (md >= 7) ? 115000000 : 9000000;
    initTable(cap);
    entries.reserve(cap); keys.reserve(cap);

    Cube slv;
    StateKey rk = getKey(slv);
    insertMap(rk, 0);
    entries.push_back({0, Move::U});
    keys.push_back(rk);

    queue<BFSNode> q;
    q.push({slv, 0, Move::U});
    MoveGenerator gen;

    long long np = 0;
    while(!q.empty()) {
        BFSNode cur = q.front();
        q.pop();

        if(cur.d >= md) continue;

        uint32_t cIdx = findMap(getKey(cur.cb));
        Node n; n.cube = cur.cb; n.depth = cur.d; n.lastMove = cur.lm;
        vector<Node> ch = gen.generateChildren(n);

        for(const Node& c : ch) {
            StateKey k = getKey(c.cube);
            if(findMap(k) != 0xFFFFFFFF) continue;

            uint32_t nIdx = static_cast<uint32_t>(entries.size());
            insertMap(k, nIdx);
            entries.push_back({cIdx, inverseMove(c.lastMove)});
            keys.push_back(k);
            q.push({c.cube, c.depth, c.lastMove});
        }
        np++;
        if (np % 1000000 == 0) cout << "\r" << cnt << flush;
    }
    cout << "\r" << cnt << "      \n";
}

bool StateDatabase::save(const string& fn) const {
    ofstream out(fn, ios::binary);
    if(!out) return false;
    uint64_t ec = entries.size();
    uint64_t kc = keys.size();
    out.write(reinterpret_cast<const char*>(&ec), sizeof(ec));
    out.write(reinterpret_cast<const char*>(entries.data()), ec * sizeof(DBEntry));
    out.write(reinterpret_cast<const char*>(&kc), sizeof(kc));
    out.write(reinterpret_cast<const char*>(keys.data()), kc * sizeof(StateKey));
    return out.good();
}

bool StateDatabase::load(const string& fn) {
    ifstream in(fn, ios::binary);
    if(!in) return false;
    table.clear(); entries.clear(); keys.clear();

    uint64_t ec;
    if(!in.read(reinterpret_cast<char*>(&ec), sizeof(ec))) return false;
    entries.resize(ec);
    if(!in.read(reinterpret_cast<char*>(entries.data()), ec * sizeof(DBEntry))) return false;

    uint64_t kc;
    if(!in.read(reinterpret_cast<char*>(&kc), sizeof(kc))) return false;
    keys.resize(kc);
    if(!in.read(reinterpret_cast<char*>(keys.data()), kc * sizeof(StateKey))) return false;

    initTable(keys.size());
    for(uint32_t i = 0; i < keys.size(); i++) {
        insertMap(keys[i], i);
    }
    return true;
}

int StateDatabase::getDistance(const Cube& cb) const {
    uint32_t idx = findMap(getKey(cb));
    if(idx == 0xFFFFFFFF) return -1;
    int d = 0;
    while(idx != 0) {
        d++;
        idx = entries[idx].pIdx;
    }
    return d;
}