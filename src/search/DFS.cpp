#ifndef DFS_H
#define DFS_H

#include "Node.h"
#include <vector>

using namespace std;

class DFS
{
public:
    bool search(Node node,
                int maxDepth,
                vector<Move>& solution);
};

#endif