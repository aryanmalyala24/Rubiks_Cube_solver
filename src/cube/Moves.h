#ifndef MOVES_H
#define MOVES_H

#include <string>

using namespace std;

enum class Move
{
    U, Ui, U2,
    D, Di, D2,
    R, Ri, R2,
    L, Li, L2,
    F, Fi, F2,
    B, Bi, B2
};

string moveToString(Move move);
Move inverseMove(Move move);

#endif