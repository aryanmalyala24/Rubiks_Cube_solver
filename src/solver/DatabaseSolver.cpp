#include "DatabaseSolver.h"

#include "../search/MoveGenerator.h"

#include <queue>

using namespace std;

DatabaseSolver::DatabaseSolver(
    StateDatabase* db
)
{
    database = db;
}

vector<Move> DatabaseSolver::solve(
    const Cube& cube,
    int maxDepth
)
{
    queue<Node> q;

    Node root;

    root.cube = cube;
    root.depth = 0;

    q.push(root);

    MoveGenerator generator;

    while(!q.empty())
    {
        Node current =
            q.front();

        q.pop();

        if(database->contains(
            current.cube))
        {
            vector<Move> solution =
                current.path;

            vector<Move> tail =
                database->getSolution(
                    current.cube);

            solution.insert(
                solution.end(),
                tail.begin(),
                tail.end());

            return solution;
        }

        if(current.depth >= maxDepth)
        {
            continue;
        }

        vector<Node> children =
            generator.generateChildren(
                current);

        for(Node child : children)
        {
            q.push(child);
        }
    }

    return {};
}