#include "test_p4.h"

#include "../src/puissance_quatre.c" // Obligatoire pour les fonctions statiques

void test_testAlign(void){
    Plateau p;
    for (int i = 0; i < NB_LIGNE; i++) {
        for (int j = 0; j < NB_COLONNE; j++) {
            p[i][j] = VIDE;
        }
    }
    p[0][0] = J1;
    p[1][1] = J1;
    p[2][2] = J1;
    p[3][3] = J1;
    CU_ASSERT(testAlign(p, 0, 0, 1, 1) == true);
}