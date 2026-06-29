#include "Scrambler.h"
#include <random>

using namespace std;

char getFace(Move move)
{
    switch (move)
    {
        case Move::U:
        case Move::Ui:
        case Move::U2:
            return 'U';

        case Move::D:
        case Move::Di:
        case Move::D2:
            return 'D';

        case Move::R:
        case Move::Ri:
        case Move::R2:
            return 'R';

        case Move::L:
        case Move::Li:
        case Move::L2:
            return 'L';

        case Move::F:
        case Move::Fi:
        case Move::F2:
            return 'F';

        case Move::B:
        case Move::Bi:
        case Move::B2:
            return 'B';
    }

    return 'X';
}

vector<Move> Scrambler::generateScramble(int length)
{
    vector<Move> scramble;

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dist(0, 17);

    Move previousMove;
    bool firstMove = true;

    for (int i = 0; i < length; i++)
    {
        Move currentMove;

        do
        {
            currentMove = static_cast<Move>(dist(gen));
        }
        while (!firstMove &&
               getFace(currentMove) == getFace(previousMove));

        scramble.push_back(currentMove);

        previousMove = currentMove;
        firstMove = false;
    }

    return scramble;
}

Move Scrambler::getInverseMove(Move move)
{
    switch (move)
    {
        case Move::U:  return Move::Ui;
        case Move::Ui: return Move::U;
        case Move::U2: return Move::U2;

        case Move::D:  return Move::Di;
        case Move::Di: return Move::D;
        case Move::D2: return Move::D2;

        case Move::R:  return Move::Ri;
        case Move::Ri: return Move::R;
        case Move::R2: return Move::R2;

        case Move::L:  return Move::Li;
        case Move::Li: return Move::L;
        case Move::L2: return Move::L2;

        case Move::F:  return Move::Fi;
        case Move::Fi: return Move::F;
        case Move::F2: return Move::F2;

        case Move::B:  return Move::Bi;
        case Move::Bi: return Move::B;
        case Move::B2: return Move::B2;
    }

    return Move::U;
}

vector<Move> Scrambler::generateInverse(const vector<Move>& scramble)
{
    vector<Move> inverse;

    for (int i = scramble.size() - 1; i >= 0; i--)
    {
        inverse.push_back(getInverseMove(scramble[i]));
    }

    return inverse;
}