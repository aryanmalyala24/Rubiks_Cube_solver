#include "Heuristic.h"
#include <algorithm>
#include <cstdint>

using namespace std;

static uint8_t c2i[256];
static uint8_t eD[12][36];
static uint8_t cD[8][216];

static struct I {
    I() {
        for(int i=0; i<256; ++i) c2i[i]=0;
        c2i['W']=0; c2i['Y']=1; c2i['G']=2; c2i['B']=3; c2i['O']=4; c2i['R']=5;

        for(int i=0; i<12; ++i) for(int j=0; j<36; ++j) eD[i][j]=0;
        for(int i=0; i<8; ++i) for(int j=0; j<216; ++j) cD[i][j]=0;

        int e_slots[12][4] = {{0,7,2,1}, {0,5,5,1}, {0,1,3,1}, {0,3,4,1},
                              {1,1,2,7}, {1,5,5,7}, {1,7,3,7}, {1,3,4,7},
                              {2,5,5,3}, {2,3,4,5}, {3,5,4,3}, {3,3,5,5}};
        
        int c_slots[8][6] = {{0,8,2,2,5,0}, {0,6,2,0,4,2}, {0,0,3,2,4,0}, {0,2,3,0,5,2},
                             {1,2,2,8,5,6}, {1,0,2,6,4,8}, {1,6,3,8,4,6}, {1,8,3,6,5,8}};

        char ec[12][2] = {{'W','G'}, {'W','R'}, {'W','B'}, {'W','O'},
                          {'Y','G'}, {'Y','R'}, {'Y','B'}, {'Y','O'},
                          {'G','R'}, {'G','O'}, {'B','O'}, {'B','R'}};
                          
        char cc[8][3] = {{'W','G','R'}, {'W','G','O'}, {'W','B','O'}, {'W','B','R'},
                         {'Y','G','R'}, {'Y','G','O'}, {'Y','B','O'}, {'Y','B','R'}};

        int e_perm[18][24];
        int c_perm[18][24];

        for(int m=0; m<18; ++m) {
            Cube c; 
            c.applyMove(static_cast<Move>(m));
            const char* f[6] = {c.getUFacePtr(), c.getDFacePtr(), c.getFFacePtr(), 
                                c.getBFacePtr(), c.getLFacePtr(), c.getRFacePtr()};

            for(int i=0; i<12; ++i) {
                char v0 = f[e_slots[i][0]][e_slots[i][1]];
                char v1 = f[e_slots[i][2]][e_slots[i][3]];
                for(int S=0; S<12; ++S) {
                    if(v0 == ec[S][0] && v1 == ec[S][1]) {
                        e_perm[m][2*S + 0] = 2*i + 0; e_perm[m][2*S + 1] = 2*i + 1;
                    } else if(v0 == ec[S][1] && v1 == ec[S][0]) {
                        e_perm[m][2*S + 0] = 2*i + 1; e_perm[m][2*S + 1] = 2*i + 0;
                    }
                }
            }

            for(int i=0; i<8; ++i) {
                char v[3] = {f[c_slots[i][0]][c_slots[i][1]], f[c_slots[i][2]][c_slots[i][3]], f[c_slots[i][4]][c_slots[i][5]]};
                for(int S=0; S<8; ++S) {
                    int map[3] = {-1, -1, -1};
                    for(int a=0; a<3; ++a) {
                        for(int b=0; b<3; ++b) {
                            if(v[a] == cc[S][b]) map[b] = a; 
                        }
                    }
                    if(map[0]!=-1 && map[1]!=-1 && map[2]!=-1) {
                        c_perm[m][3*S + 0] = 3*i + map[0];
                        c_perm[m][3*S + 1] = 3*i + map[1];
                        c_perm[m][3*S + 2] = 3*i + map[2];
                    }
                }
            }
        }

        for(int T=0; T<12; ++T) {
            uint8_t dist[24][24];
            for(int i=0; i<24; ++i) for(int j=0; j<24; ++j) dist[i][j] = 99;
            dist[2*T][2*T+1] = 0;
            
            int q1[128], q2[128], head=0, tail=0;
            q1[tail] = 2*T; q2[tail] = 2*T+1; tail++;

            while(head < tail) {
                int p1 = q1[head], p2 = q2[head]; head++;
                for(int m=0; m<18; ++m) {
                    int n1 = e_perm[m][p1], n2 = e_perm[m][p2];
                    if(dist[n1][n2] == 99) {
                        dist[n1][n2] = dist[p1][p2] + 1;
                        q1[tail] = n1; q2[tail] = n2; tail++;
                    }
                }
            }
            
            for(int i=0; i<12; ++i) {
                for(int a=0; a<2; ++a) {
                    for(int b=0; b<2; ++b) {
                        if(a == b) continue;
                        if(dist[2*i + a][2*i + b] != 99) {
                            uint8_t col[2] = { c2i[(uint8_t)ec[T][0]], c2i[(uint8_t)ec[T][1]] };
                            eD[i][col[a]*6 + col[b]] = dist[2*i + a][2*i + b];
                        }
                    }
                }
            }
        }

        for(int T=0; T<8; ++T) {
            uint8_t dist[24][24][24];
            for(int i=0; i<24; ++i) for(int j=0; j<24; ++j) for(int k=0; k<24; ++k) dist[i][j][k] = 99;
            dist[3*T][3*T+1][3*T+2] = 0;
            
            int q1[128], q2[128], q3[128], head=0, tail=0;
            q1[tail] = 3*T; q2[tail] = 3*T+1; q3[tail] = 3*T+2; tail++;

            while(head < tail) {
                int p1 = q1[head], p2 = q2[head], p3 = q3[head]; head++;
                for(int m=0; m<18; ++m) {
                    int n1 = c_perm[m][p1], n2 = c_perm[m][p2], n3 = c_perm[m][p3];
                    if(dist[n1][n2][n3] == 99) {
                        dist[n1][n2][n3] = dist[p1][p2][p3] + 1;
                        q1[tail] = n1; q2[tail] = n2; q3[tail] = n3; tail++;
                    }
                }
            }
            
            for(int i=0; i<8; ++i) {
                for(int a=0; a<3; ++a) {
                    for(int b=0; b<3; ++b) {
                        if(a == b) continue;
                        for(int c=0; c<3; ++c) {
                            if(a == c || b == c) continue;
                            if(dist[3*i + a][3*i + b][3*i + c] != 99) {
                                uint8_t col[3] = { c2i[(uint8_t)cc[T][0]], c2i[(uint8_t)cc[T][1]], c2i[(uint8_t)cc[T][2]] };
                                cD[i][col[a]*36 + col[b]*6 + col[c]] = dist[3*i + a][3*i + b][3*i + c];
                            }
                        }
                    }
                }
            }
        }
    }
} init;

int Heuristic::misplacedStickers(const Cube& cube)
{
    const char* U = cube.getUFacePtr();
    const char* D = cube.getDFacePtr();
    const char* F = cube.getFFacePtr();
    const char* B = cube.getBFacePtr();
    const char* L = cube.getLFacePtr();
    const char* R = cube.getRFacePtr();

    int es = 0;
    es += eD[0][c2i[(uint8_t)U[7]]*6 + c2i[(uint8_t)F[1]]];
    es += eD[1][c2i[(uint8_t)U[5]]*6 + c2i[(uint8_t)R[1]]];
    es += eD[2][c2i[(uint8_t)U[1]]*6 + c2i[(uint8_t)B[1]]];
    es += eD[3][c2i[(uint8_t)U[3]]*6 + c2i[(uint8_t)L[1]]];
    es += eD[4][c2i[(uint8_t)D[1]]*6 + c2i[(uint8_t)F[7]]];
    es += eD[5][c2i[(uint8_t)D[5]]*6 + c2i[(uint8_t)R[7]]];
    es += eD[6][c2i[(uint8_t)D[7]]*6 + c2i[(uint8_t)B[7]]];
    es += eD[7][c2i[(uint8_t)D[3]]*6 + c2i[(uint8_t)L[7]]];
    es += eD[8][c2i[(uint8_t)F[5]]*6 + c2i[(uint8_t)R[3]]];
    es += eD[9][c2i[(uint8_t)F[3]]*6 + c2i[(uint8_t)L[5]]];
    es += eD[10][c2i[(uint8_t)B[5]]*6 + c2i[(uint8_t)L[3]]];
    es += eD[11][c2i[(uint8_t)B[3]]*6 + c2i[(uint8_t)R[5]]];

    int cs = 0;
    cs += cD[0][c2i[(uint8_t)U[8]]*36 + c2i[(uint8_t)F[2]]*6 + c2i[(uint8_t)R[0]]];
    cs += cD[1][c2i[(uint8_t)U[6]]*36 + c2i[(uint8_t)F[0]]*6 + c2i[(uint8_t)L[2]]];
    cs += cD[2][c2i[(uint8_t)U[0]]*36 + c2i[(uint8_t)B[2]]*6 + c2i[(uint8_t)L[0]]];
    cs += cD[3][c2i[(uint8_t)U[2]]*36 + c2i[(uint8_t)B[0]]*6 + c2i[(uint8_t)R[2]]];
    cs += cD[4][c2i[(uint8_t)D[2]]*36 + c2i[(uint8_t)F[8]]*6 + c2i[(uint8_t)R[6]]];
    cs += cD[5][c2i[(uint8_t)D[0]]*36 + c2i[(uint8_t)F[6]]*6 + c2i[(uint8_t)L[8]]];
    cs += cD[6][c2i[(uint8_t)D[6]]*36 + c2i[(uint8_t)B[8]]*6 + c2i[(uint8_t)L[6]]];
    cs += cD[7][c2i[(uint8_t)D[8]]*36 + c2i[(uint8_t)B[6]]*6 + c2i[(uint8_t)R[8]]];

    return max(es, cs);
}