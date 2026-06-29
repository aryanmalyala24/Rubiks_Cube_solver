#ifndef SCRAMBLER_H
#define SCRAMBLER_H

#include <vector>
#include "../cube/Moves.h"

using namespace std;

class Scrambler
{
public:
    vector<Move> generateScramble(int length);
    Move getInverseMove(Move move);

    vector<Move> generateInverse(const vector<Move>& scramble);
};

#endif