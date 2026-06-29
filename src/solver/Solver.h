#ifndef SOLVER_H
#define SOLVER_H

#include "../cube/Cube.h"

#include "../database/StateDatabase.h"
#include "../search/IDAStar.h"

#include "SolveResult.h"

class Solver
{
private:
    StateDatabase database;

    IDAStar ida;

public:
    Solver();

    SolveResult solve(
        const Cube& cube
    );
};

#endif