#include "Solver.h"

#include "../search/Node.h"

#include <iostream>
#include <chrono>

using namespace std;

Solver::Solver()
    : ida(&database)
{
    if(database.load("database.bin"))
    {
        cout
            << "Database loaded!\n";
    }
    else
    {
        cout
            << "Building database...\n";

        database.build(7);

        database.save("database.bin");

        cout
            << "Database saved!\n";
    }

    cout
        << "States: "
        << database.size()
        << "\n";
}

SolveResult Solver::solve(
    const Cube& cube
)
{
    SolveResult result;

    Node root;

    root.cube = cube;

    auto start =
        chrono::high_resolution_clock::now();

    vector<Move> solution =
        ida.solve(root);

    auto end =
        chrono::high_resolution_clock::now();

    SearchStats stats =
        ida.getStats();

    result.solution =
        solution;

    result.nodesExpanded =
        stats.nodesExpanded;

    result.solveTimeMs =
        chrono::duration_cast
        <
            chrono::milliseconds
        >
        (
            end - start
        ).count();

    result.solved =
        !solution.empty()
        || cube.isSolved();

    return result;
}