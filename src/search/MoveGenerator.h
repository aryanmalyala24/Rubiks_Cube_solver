#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "Node.h"
#include <vector>

using namespace std;

class MoveGenerator
{
public:
    vector<Node> generateChildren(const Node& node);
    bool isValidSuccessor(Move last, Move current);
};

#endifa#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "Node.h"
#include <vector>

using namespace std;

class MoveGenerator
{
public:
    vector<Node> generateChildren(const Node& node);
    bool isValidSuccessor(Move last, Move current);
};

#endif