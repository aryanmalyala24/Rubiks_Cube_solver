#ifndef DATABASESOLVER_H
#define DATABASESOLVER_H

#include "../database/StateDatabase.h"
#include "../search/Node.h"

#include <vector>

using namespace std;

class DatabaseSolver
{
private:
    StateDatabase* database;

public:
    DatabaseSolver(StateDatabase* db);

    vector<Move> solve(
        const Cube& cube,
        int maxDepth = 6
    );
};

#endif