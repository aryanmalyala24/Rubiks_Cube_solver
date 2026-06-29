#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <string>
#include <cstdint>
#include "Moves.h"

using namespace std;

class Cube {
private:
    char U_face[9];
    char D_face[9];
    char F_face[9];
    char B_face[9];
    char L_face[9];
    char R_face[9];

    void rotateFaceClockwise(char face[9]);
    void recomputeHash();
    
    // Zobrist Incremental Helpers
    uint64_t hash_U() const;
    uint64_t hash_D() const;
    uint64_t hash_R() const;
    uint64_t hash_L() const;
    uint64_t hash_F() const;
    uint64_t hash_B() const;

public:
    uint64_t hash; // Precomputed hash
    Cube();

    void display() const;
    void applyMove(Move move);
    
    void U(); void Ui(); void U2();
    void D(); void Di(); void D2();
    void R(); void Ri(); void R2();
    void L(); void Li(); void L2();
    void F(); void Fi(); void F2();
    void B(); void Bi(); void B2();

    string encodeState() const;
    bool isSolved() const;
    void reset();

    const char* getUFacePtr() const { return U_face; }
    const char* getDFacePtr() const { return D_face; }
    const char* getFFacePtr() const { return F_face; }
    const char* getBFacePtr() const { return B_face; }
    const char* getLFacePtr() const { return L_face; }
    const char* getRFacePtr() const { return R_face; }

    vector<char> getUFace() const { return vector<char>(U_face, U_face + 9); }
    vector<char> getDFace() const { return vector<char>(D_face, D_face + 9); }
    vector<char> getFFace() const { return vector<char>(F_face, F_face + 9); }
    vector<char> getBFace() const { return vector<char>(B_face, B_face + 9); }
    vector<char> getLFace() const { return vector<char>(L_face, L_face + 9); }
    vector<char> getRFace() const { return vector<char>(R_face, R_face + 9); }
};

#endif