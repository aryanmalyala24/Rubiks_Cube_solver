#ifndef SOLVE_RESULT_H
#define SOLVE_RESULT_H

#include <vector>

#include "../cube/Moves.h"

using namespace std;

struct SolveResult
{
    vector<Move> solution;

    long long nodesExpanded = 0;

    long long solveTimeMs = 0;

    bool solved = false;
};

#endif