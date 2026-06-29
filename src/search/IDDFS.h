#ifndef IDDFS_H
#define IDDFS_H

#include "Node.h"
#include <vector>

using namespace std;

class IDDFS
{
public:
    vector<Move> solve(Node root,
                       int maxDepth);
};

#endif