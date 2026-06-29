#ifndef IDASTAR_H
#define IDASTAR_H

#include "Node.h"
#include "Heuristic.h"
#include "SearchStats.h"
#include "../database/StateDatabase.h"

using namespace std;

class IDAStar
{
private:
    Heuristic heuristic;
    SearchStats stats;
    StateDatabase* database;

    bool search(Node& node,
                int threshold,
                vector<Move>& solution);

public:
    IDAStar(StateDatabase* db = nullptr);
    
    vector<Move> solve(Node root);

    SearchStats getStats() const;
};

#endif