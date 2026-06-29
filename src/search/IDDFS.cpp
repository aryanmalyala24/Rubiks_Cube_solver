#include "IDDFS.h"
#include "DFS.h"

using namespace std;

vector<Move> IDDFS::solve(Node root,
                          int maxDepth)
{
    DFS dfs;

    vector<Move> solution;

    for(int depth = 0;
        depth <= maxDepth;
        depth++)
    {
        solution.clear();

        if(dfs.search(root,
                      depth,
                      solution))
        {
            return solution;
        }
    }

    return {};
}