#include "Cube.h"
#include "Moves.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

// Static Zobrist Hash Table
static uint64_t zh[6][9][256];
static bool zi = false;

static void initZobrist() {
    if(zi) return;
    // Static seed ensures saved database files match across program runs
    mt19937_64 rng(1337); 
    for(int f=0; f<6; ++f)
        for(int i=0; i<9; ++i)
            for(int c=0; c<256; ++c)
                zh[f][i][c] = rng();
    zi = true;
}

Cube::Cube() { 
    initZobrist(); 
    reset(); 
}

void printFace(const char* face) {
    for (int i = 0; i < 9; i++) {
        cout << face[i] << " ";
        if ((i + 1) % 3 == 0) cout << endl;
    }
}

void Cube::display() const {
    cout << "\nUP\n"; printFace(U_face);
    cout << "\nDOWN\n"; printFace(D_face);
    cout << "\nFRONT\n"; printFace(F_face);
    cout << "\nBACK\n"; printFace(B_face);
    cout << "\nLEFT\n"; printFace(L_face);
    cout << "\nRIGHT\n"; printFace(R_face);
}

void Cube::rotateFaceClockwise(char face[9]) {
    char temp[9];
    copy(face, face + 9, temp);
    face[0] = temp[6]; face[1] = temp[3]; face[2] = temp[0];
    face[3] = temp[7]; face[4] = temp[4]; face[5] = temp[1];
    face[6] = temp[8]; face[7] = temp[5]; face[8] = temp[2];
}

void Cube::recomputeHash() {
    hash = 0;
    const char* fcs[6] = {U_face, D_face, F_face, B_face, L_face, R_face};
    for(int f=0; f<6; ++f)
        for(int i=0; i<9; ++i)
            hash ^= zh[f][i][(uint8_t)fcs[f][i]];
}

// --- Zobrist Wrappers (Reads exactly the 20 stickers affected by each move) ---

uint64_t Cube::hash_U() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[0][i][(uint8_t)U_face[i]];
    for(int i=0; i<3; ++i) {
        h ^= zh[2][i][(uint8_t)F_face[i]] ^ zh[4][i][(uint8_t)L_face[i]];
        h ^= zh[3][i][(uint8_t)B_face[i]] ^ zh[5][i][(uint8_t)R_face[i]];
    }
    return h;
}
uint64_t Cube::hash_D() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[1][i][(uint8_t)D_face[i]];
    for(int i=6; i<9; ++i) {
        h ^= zh[2][i][(uint8_t)F_face[i]] ^ zh[5][i][(uint8_t)R_face[i]];
        h ^= zh[3][i][(uint8_t)B_face[i]] ^ zh[4][i][(uint8_t)L_face[i]];
    }
    return h;
}
uint64_t Cube::hash_R() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[5][i][(uint8_t)R_face[i]];
    h ^= zh[0][2][(uint8_t)U_face[2]] ^ zh[0][5][(uint8_t)U_face[5]] ^ zh[0][8][(uint8_t)U_face[8]];
    h ^= zh[2][2][(uint8_t)F_face[2]] ^ zh[2][5][(uint8_t)F_face[5]] ^ zh[2][8][(uint8_t)F_face[8]];
    h ^= zh[1][2][(uint8_t)D_face[2]] ^ zh[1][5][(uint8_t)D_face[5]] ^ zh[1][8][(uint8_t)D_face[8]];
    h ^= zh[3][6][(uint8_t)B_face[6]] ^ zh[3][3][(uint8_t)B_face[3]] ^ zh[3][0][(uint8_t)B_face[0]];
    return h;
}
uint64_t Cube::hash_L() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[4][i][(uint8_t)L_face[i]];
    h ^= zh[0][0][(uint8_t)U_face[0]] ^ zh[0][3][(uint8_t)U_face[3]] ^ zh[0][6][(uint8_t)U_face[6]];
    h ^= zh[3][8][(uint8_t)B_face[8]] ^ zh[3][5][(uint8_t)B_face[5]] ^ zh[3][2][(uint8_t)B_face[2]];
    h ^= zh[1][0][(uint8_t)D_face[0]] ^ zh[1][3][(uint8_t)D_face[3]] ^ zh[1][6][(uint8_t)D_face[6]];
    h ^= zh[2][0][(uint8_t)F_face[0]] ^ zh[2][3][(uint8_t)F_face[3]] ^ zh[2][6][(uint8_t)F_face[6]];
    return h;
}
uint64_t Cube::hash_F() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[2][i][(uint8_t)F_face[i]];
    h ^= zh[0][6][(uint8_t)U_face[6]] ^ zh[0][7][(uint8_t)U_face[7]] ^ zh[0][8][(uint8_t)U_face[8]];
    h ^= zh[4][8][(uint8_t)L_face[8]] ^ zh[4][5][(uint8_t)L_face[5]] ^ zh[4][2][(uint8_t)L_face[2]];
    h ^= zh[1][2][(uint8_t)D_face[2]] ^ zh[1][1][(uint8_t)D_face[1]] ^ zh[1][0][(uint8_t)D_face[0]];
    h ^= zh[5][0][(uint8_t)R_face[0]] ^ zh[5][3][(uint8_t)R_face[3]] ^ zh[5][6][(uint8_t)R_face[6]];
    return h;
}
uint64_t Cube::hash_B() const {
    uint64_t h = 0;
    for(int i=0; i<9; ++i) h ^= zh[3][i][(uint8_t)B_face[i]];
    h ^= zh[0][0][(uint8_t)U_face[0]] ^ zh[0][1][(uint8_t)U_face[1]] ^ zh[0][2][(uint8_t)U_face[2]];
    h ^= zh[5][2][(uint8_t)R_face[2]] ^ zh[5][5][(uint8_t)R_face[5]] ^ zh[5][8][(uint8_t)R_face[8]];
    h ^= zh[1][8][(uint8_t)D_face[8]] ^ zh[1][7][(uint8_t)D_face[7]] ^ zh[1][6][(uint8_t)D_face[6]];
    h ^= zh[4][6][(uint8_t)L_face[6]] ^ zh[4][3][(uint8_t)L_face[3]] ^ zh[4][0][(uint8_t)L_face[0]];
    return h;
}


// --- Safe, Proven Moves wrapped in O(1) Hashing ---

void Cube::U() {
    hash ^= hash_U();
    char temp[3] = { F_face[0], F_face[1], F_face[2] };
    F_face[0] = L_face[0]; F_face[1] = L_face[1]; F_face[2] = L_face[2];
    L_face[0] = B_face[0]; L_face[1] = B_face[1]; L_face[2] = B_face[2];
    B_face[0] = R_face[0]; B_face[1] = R_face[1]; B_face[2] = R_face[2];
    R_face[0] = temp[0];   R_face[1] = temp[1];   R_face[2] = temp[2];
    rotateFaceClockwise(U_face);
    hash ^= hash_U();
}
void Cube::Ui() { U(); U(); U(); }
void Cube::U2() { U(); U(); }

void Cube::D() {
    hash ^= hash_D();
    char temp[3] = { F_face[6], F_face[7], F_face[8] };
    F_face[6] = R_face[6]; F_face[7] = R_face[7]; F_face[8] = R_face[8];
    R_face[6] = B_face[6]; R_face[7] = B_face[7]; R_face[8] = B_face[8];
    B_face[6] = L_face[6]; B_face[7] = L_face[7]; B_face[8] = L_face[8];
    L_face[6] = temp[0];   L_face[7] = temp[1];   L_face[8] = temp[2];
    rotateFaceClockwise(D_face);
    hash ^= hash_D();
}
void Cube::Di() { D(); D(); D(); }
void Cube::D2() { D(); D(); }

void Cube::R() {
    hash ^= hash_R();
    char temp[3] = { U_face[2], U_face[5], U_face[8] };
    U_face[2] = F_face[2]; U_face[5] = F_face[5]; U_face[8] = F_face[8];
    F_face[2] = D_face[2]; F_face[5] = D_face[5]; F_face[8] = D_face[8];
    D_face[2] = B_face[6]; D_face[5] = B_face[3]; D_face[8] = B_face[0];
    B_face[6] = temp[0];   B_face[3] = temp[1];   B_face[0] = temp[2];
    rotateFaceClockwise(R_face);
    hash ^= hash_R();
}
void Cube::Ri() { R(); R(); R(); }
void Cube::R2() { R(); R(); }

void Cube::L() {
    hash ^= hash_L();
    char temp[3] = { U_face[0], U_face[3], U_face[6] };
    U_face[0] = B_face[8]; U_face[3] = B_face[5]; U_face[6] = B_face[2];
    B_face[8] = D_face[0]; B_face[5] = D_face[3]; B_face[2] = D_face[6];
    D_face[0] = F_face[0]; D_face[3] = F_face[3]; D_face[6] = F_face[6];
    F_face[0] = temp[0];   F_face[3] = temp[1];   F_face[6] = temp[2];
    rotateFaceClockwise(L_face);
    hash ^= hash_L();
}
void Cube::Li() { L(); L(); L(); }
void Cube::L2() { L(); L(); }

void Cube::F() {
    hash ^= hash_F();
    char temp[3] = { U_face[6], U_face[7], U_face[8] };
    U_face[6] = L_face[8]; U_face[7] = L_face[5]; U_face[8] = L_face[2];
    L_face[8] = D_face[2]; L_face[5] = D_face[1]; L_face[2] = D_face[0];
    D_face[2] = R_face[0]; D_face[1] = R_face[3]; D_face[0] = R_face[6];
    R_face[0] = temp[0];   R_face[3] = temp[1];   R_face[6] = temp[2];
    rotateFaceClockwise(F_face);
    hash ^= hash_F();
}
void Cube::Fi() { F(); F(); F(); }
void Cube::F2() { F(); F(); }

void Cube::B() {
    hash ^= hash_B();
    char temp[3] = { U_face[0], U_face[1], U_face[2] };
    U_face[0] = R_face[2]; U_face[1] = R_face[5]; U_face[2] = R_face[8];
    R_face[2] = D_face[8]; R_face[5] = D_face[7]; R_face[8] = D_face[6];
    D_face[8] = L_face[6]; D_face[7] = L_face[3]; D_face[6] = L_face[0];
    L_face[6] = temp[0];   L_face[3] = temp[1];   L_face[0] = temp[2];
    rotateFaceClockwise(B_face);
    hash ^= hash_B();
}
void Cube::Bi() { B(); B(); B(); }
void Cube::B2() { B(); B(); }

bool Cube::isSolved() const {
    char u = U_face[4], d = D_face[4], f = F_face[4], b = B_face[4], l = L_face[4], r = R_face[4];
    for (int i = 0; i < 9; i++) {
        if (U_face[i] != u || D_face[i] != d || F_face[i] != f ||
            B_face[i] != b || L_face[i] != l || R_face[i] != r) return false;
    }
    return true;
}

void Cube::reset() {
    fill(U_face, U_face + 9, 'W'); fill(D_face, D_face + 9, 'Y');
    fill(F_face, F_face + 9, 'G'); fill(B_face, B_face + 9, 'B');
    fill(L_face, L_face + 9, 'O'); fill(R_face, R_face + 9, 'R');
    recomputeHash(); // Only executed during a hard reset
}

void Cube::applyMove(Move m) {
    switch (m) {
        case Move::U:  U();  break; case Move::Ui: Ui(); break; case Move::U2: U2(); break;
        case Move::D:  D();  break; case Move::Di: Di(); break; case Move::D2: D2(); break;
        case Move::R:  R();  break; case Move::Ri: Ri(); break; case Move::R2: R2(); break;
        case Move::L:  L();  break; case Move::Li: Li(); break; case Move::L2: L2(); break;
        case Move::F:  F();  break; case Move::Fi: Fi(); break; case Move::F2: F2(); break;
        case Move::B:  B();  break; case Move::Bi: Bi(); break; case Move::B2: B2(); break;
    }
}

string Cube::encodeState() const {
    string s; s.reserve(54);
    for(int i=0; i<9; ++i) s += U_face[i];
    for(int i=0; i<9; ++i) s += D_face[i];
    for(int i=0; i<9; ++i) s += F_face[i];
    for(int i=0; i<9; ++i) s += B_face[i];
    for(int i=0; i<9; ++i) s += L_face[i];
    for(int i=0; i<9; ++i) s += R_face[i];
    return s;
}