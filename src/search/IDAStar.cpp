#include "IDAStar.h"
#include <algorithm>

using namespace std;

IDAStar::IDAStar(StateDatabase* d) {
    database = d;
}

bool IDAStar::search(Node& n, int t, vector<Move>& s) {
    int raw_h = heuristic.misplacedStickers(n.cube);
    int h = (raw_h + 3) / 4;
    int d = -1;

    if (database) {
        if (t - n.depth <= 7) {
            d = database->getDistance(n.cube);
            if (d != -1) h = d;
            else if (h < 8) h = 8;
        } else {
            if (h < 8) h = 8;
        }
    }

    int c = n.depth + h;
    if (c > t) return false;

    stats.nodesExpanded++;

    if (d != -1) {
        s = n.path;
        vector<Move> l = database->getSolution(n.cube);
        s.insert(s.end(), l.begin(), l.end());
        return true;
    }

    if (n.cube.isSolved()) {
        s = n.path;
        return true;
    }

    if (n.depth > 20) return false;

    static const Move moves[] = {
        Move::U, Move::Ui, Move::U2, Move::D, Move::Di, Move::D2,
        Move::R, Move::Ri, Move::R2, Move::L, Move::Li, Move::L2,
        Move::F, Move::Fi, Move::F2, Move::B, Move::Bi, Move::B2
    };

    struct MoveOrder {
        Move mv;
        int h_val;
    };
    MoveOrder valid_moves[18];
    int move_count = 0;

    int lastFace = (n.depth > 0) ? static_cast<int>(n.lastMove) / 3 : -1;

    for (Move move : moves) {
        if (n.depth > 0) {
            int currentFace = static_cast<int>(move) / 3;
            if (lastFace == currentFace) continue;
            if (lastFace / 2 == currentFace / 2 && currentFace < lastFace) continue;
        }

        n.cube.applyMove(move);
        int child_raw = heuristic.misplacedStickers(n.cube);
        int child_h = (child_raw + 3) / 4;
        n.cube.applyMove(inverseMove(move));

        valid_moves[move_count++] = {move, child_h};
    }

    for (int i = 0; i < move_count - 1; ++i) {
        int best_idx = i;
        for (int j = i + 1; j < move_count; ++j) {
            if (valid_moves[j].h_val < valid_moves[best_idx].h_val) {
                best_idx = j;
            }
        }
        if (best_idx != i) {
            swap(valid_moves[i], valid_moves[best_idx]);
        }
    }

    for (int i = 0; i < move_count; ++i) {
        Move next_move = valid_moves[i].mv;
        
        n.cube.applyMove(next_move);
        n.path.push_back(next_move);
        Move old_last = n.lastMove;
        n.lastMove = next_move;
        n.depth++;

        if (search(n, t, s)) return true;

        n.depth--;
        n.lastMove = old_last;
        n.path.pop_back();
        n.cube.applyMove(inverseMove(next_move));
    }

    return false;
}

vector<Move> IDAStar::solve(Node r) {
    stats.nodesExpanded = 0;

    int t = (heuristic.misplacedStickers(r.cube) + 3) / 4;
    
    if (database) {
        int d = database->getDistance(r.cube);
        if (d != -1) t = d;
        else if (t < 8) t = 8;
    }

    vector<Move> s;

    while (true) {
        s.clear();
        if (search(r, t, s)) return s;
        t++;
    }
}

SearchStats IDAStar::getStats() const {
    return stats;
}