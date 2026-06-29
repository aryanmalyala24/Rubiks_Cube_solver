#ifndef NODE_H
#define NODE_H

#include "../cube/Cube.h"
#include "../cube/Moves.h"

#include <vector>

using namespace std;

class Node
{
public:
    Cube cube;

    vector<Move> path;

    Move lastMove;

    int depth;

    Node();
};

#endif