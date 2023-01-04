#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/puissance_quatre.h"
#include "test_zoe.h"

Puissance4 *jeu;
userInterface *ui;

int initSuiteBeginning(void) {
  jeu = initPuissance4();
  if (!jeu)
    return CUE_NOMEMORY;
  jeu->j1 = malloc(sizeof(Joueur));
  if (!jeu->j1)
    return CUE_NOMEMORY;
  jeu->j1->type = J1;
  jeu->j2 = malloc(sizeof(Joueur));
  if (!jeu->j2)
    return CUE_NOMEMORY;
  jeu->j2->type = J2;
  return CUE_SUCCESS;
}
int cleanSuite(void) {
  clean(jeu, ui);
  return CUE_SUCCESS;
}

// suite Beginning :
void test_initGame(void) {
  CU_ASSERT_PTR_NOT_NULL(jeu);
  initGame(jeu);
  CU_ASSERT_PTR_NOT_NULL(jeu->j1);
  CU_ASSERT_PTR_NOT_NULL(jeu->j2);
  CU_ASSERT_PTR_NOT_NULL(jeu->courant);
  CU_ASSERT_PTR_EQUAL(jeu->courant, jeu->j2);
  CU_ASSERT_PTR_NOT_EQUAL(jeu->j1, jeu->j2);
  CU_ASSERT_FALSE(jeu->rageQuit);
  CU_ASSERT_EQUAL(jeu->nb_jetons, 0);
  CU_ASSERT_EQUAL(jeu->j1->type, J1);
  CU_ASSERT_EQUAL(jeu->j2->type, J2);
  CU_ASSERT_EQUAL(jeu->courant->type, J2);
}
void test_plateauVide(void) {
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      CU_ASSERT_EQUAL(jeu->plateau[i][j], VIDE);
    }
  }
}
void test_descendDerniereLigne(void) {
  for (int i = 0; i < NB_COLONNE; i++) {
    CU_ASSERT_EQUAL(testColonne(jeu->plateau, i), NB_LIGNE - 1);
  }
}
/**
 * @brief Ajoute puis enlève deux jetons.
 *
 */
void test_ajoutEtSuppr2Pions(void) {
  modifJeton(jeu, NB_LIGNE - 1, 3, J1);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 1][3], J1);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 2);

  modifJeton(jeu, NB_LIGNE - 2, 3, J2);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 2][3], J2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 3);

  modifJeton(jeu, NB_LIGNE - 2, 3, VIDE);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 2][3], VIDE);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 2);

  modifJeton(jeu, NB_LIGNE - 1, 3, VIDE);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 1][3], VIDE);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 1);
}
void test_alignement1Jeton(void) {
  jeu->plateau[NB_LIGNE - 1][4] = J1;
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 0, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, 0));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, -1));
  CU_ASSERT_FALSE(testEnd(jeu, NB_LIGNE - 1, 4));
}

// suite BASIQUE :

// changer joueur : j1 à j2 et j2 à j1
// test Colonne :colonne pleine -> -1
// test end
// test align: toutes directions pas fin

// suite FIN :

// modif jeton
// test end
// test align
// test toutes colonne pleine

static CU_TestInfo test_array_Beginning[] = {
    {"vérifie que le jeu est bien initialisé", test_initGame},
    {"vérifie que le plateau est bien initialisé", test_plateauVide},
    {"vérifie que les pions descendent tout en bas de la grille",
     test_descendDerniereLigne},
    {"vérifie que les ajouts (et les suppressions) de pions fonctionnent et "
     "que les calculs qui en découlent sont bons",
     test_ajoutEtSuppr2Pions},
    {"vérifie qu'après les ajouts et suppressions le plateau est de nouveau "
     "à vide",
     test_plateauVide},
    {"ajoute un jeton et test ses alignements", test_alignement1Jeton},
    CU_TEST_INFO_NULL};

// static CU_TestInfo test_array_Basique[] = {CU_TEST_INFO_NULL};

// static CU_TestInfo test_array_Fin[] = {CU_TEST_INFO_NULL};

static CU_SuiteInfo suites[2] = {{"suiteBeginning", initSuiteBeginning,
                                  cleanSuite, NULL, NULL, test_array_Beginning},
                                 //{"suiteBasique"}

                                 CU_SUITE_INFO_NULL};

CU_SuiteInfo *getTestZoeSuites() { return suites; }