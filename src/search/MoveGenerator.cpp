#include "MoveGenerator.h"

using namespace std;

bool MoveGenerator::isValidSuccessor(Move last, Move current)
{
    int lastFace = static_cast<int>(last) / 3;
    int currentFace = static_cast<int>(current) / 3;

    if (lastFace == currentFace) 
    {
        return false;
    }

    if (lastFace / 2 == currentFace / 2) 
    {
        if (currentFace < lastFace) 
        {
            return false;
        }
    }

    return true;
}

vector<Node> MoveGenerator::generateChildren(const Node& node)
{
    vector<Node> children;
    children.reserve(18);

    static const Move moves[] =
    {
        Move::U, Move::Ui, Move::U2, Move::D, Move::Di, Move::D2,
        Move::R, Move::Ri, Move::R2, Move::L, Move::Li, Move::L2,
        Move::F, Move::Fi, Move::F2, Move::B, Move::Bi, Move::B2
    };

    for (Move move : moves)
    {   
        if (node.depth > 0 && !isValidSuccessor(node.lastMove, move))
        {
            continue;
        }
        
        Node child = node;
        child.cube.applyMove(move);
        child.path.push_back(move);
        child.lastMove = move;
        child.depth++;

        children.push_back(child);
    }

    return children;
}