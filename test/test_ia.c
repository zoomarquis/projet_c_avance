/**
 * @file test_ia.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Tests unitaires du fichier ia.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/ia.h"
#include "test_ia.h"
#include "test_p4.h"

/**
 * @brief Un pointeur sur le jeu (extern : celui dans test_p4.c)
 *
 */
extern Puissance4 *jeu;
/**
 * @brief Un pointeur sur une interface (extern : celui dans test_p4.c)
 *
 */
extern userInterface *ui;

/*
plateau 1:
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   | X |   |   |   |   |
+---+---+---+---+---+---+---+
|   | X | O | O | O |   | X |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7

plateau 2:
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   | X |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   | O | O |   |   |   |
+---+---+---+---+---+---+---+
|   |   | X | X |   |   |   |
+---+---+---+---+---+---+---+
|   | X | O | X | O | O | X |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7
*/

/**
 * @brief Vérifie que la fonction valeurCase est fonctionnelle.
 *
 */
void test_valeurCase(void) {
  jeu->plateau[5][1] = J1;
  jeu->plateau[5][2] = J2;
  jeu->plateau[4][2] = J1;
  jeu->plateau[5][3] = J2;
  jeu->plateau[5][4] = J2;
  jeu->plateau[5][6] = J1; // plateau 1

  jeu->courant = jeu->j1; // X
  CU_ASSERT_EQUAL(0, valeurCase(*jeu, NB_LIGNE, 0));
  CU_ASSERT_EQUAL(0, valeurCase(*jeu, 2, NB_COLONNE));

  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 0), 1); // vide
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 1), 2); // X
  CU_ASSERT_EQUAL(valeurCase(*jeu, 4, 2), 2); // X
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 2), 0); // O

  jeu->courant = jeu->j2;                     // O
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 5), 1); // vide
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 1), 0); // X
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 3), 2); // O
  CU_ASSERT_EQUAL(valeurCase(*jeu, 5, 6), 0); // X
}

/**
 * @brief Vérifie que la fonction autour est fonctionnelle pour un plateau
 * donné.
 *
 */
void test_autourP1(void) {
  /* plateau 1 :
  J1 : ligne - colonne
  5 - 1 = 1 + 1 + 1 + 2 + 0 = 5
  4 - 2 = 7
  5 - 6 = 1 + 1 + 1 = 3
  J2 :
  5 - 2 = 4
  5 - 3 = 6
  5 - 4 = 6
  */
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(autour(*jeu, 5, 1), 5);
  CU_ASSERT_EQUAL(autour(*jeu, 4, 2), 7);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 6), 3);

  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(autour(*jeu, 5, 2), 4);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 3), 6);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 4), 6);
}

/**
 * @brief Vérifie que la fonction scoreJoueur est fonctionnelle pour un plateau
 * donné.
 *
 */
void test_scoreJoueurP1(void) {
  // plateau 1 : somme des calculs de test_autourP1 : 15 et 16
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(scoreJoueur(*jeu), 15);
  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(scoreJoueur(*jeu), 16);
}

/**
 * @brief Vérifie que la fonction autour est fonctionnelle pour un  autre
 * plateau donné.
 *
 */
void test_autourP2(void) {
  // plateau 2
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(autour(*jeu, 5, 1), 5);
  CU_ASSERT_EQUAL(autour(*jeu, 4, 2), 8);
  CU_ASSERT_EQUAL(autour(*jeu, 2, 2), 6);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 3), 5);
  CU_ASSERT_EQUAL(autour(*jeu, 4, 3), 6);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 6), 2);

  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(autour(*jeu, 5, 2), 1);
  CU_ASSERT_EQUAL(autour(*jeu, 3, 2), 6);
  CU_ASSERT_EQUAL(autour(*jeu, 3, 3), 6);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 4), 4);
  CU_ASSERT_EQUAL(autour(*jeu, 5, 5), 5);
}

/**
 * @brief Vérifie que la fonction scoreJoueur est fonctionnelle pour un autre
 * plateau donné.
 *
 */
void test_scoreJoueurP2(void) {
  // plateau 2 : somme des calculs de test_autour21 : 32 et 22
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(scoreJoueur(*jeu), 32);
  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(scoreJoueur(*jeu), 22);
}

/**
 * @brief Vérifie que la fonction evaluation est fonctionnelle pour deux
 * plateaux donnés.
 *
 */
void test_evaluation(void) {
  // plateau 1 : -1 et 1 (jeu à somme nulle)
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(evaluation(jeu), 1);
  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(evaluation(jeu), -1);

  // transformation en plateau 2 :
  jeu->plateau[5][1] = J1;

  jeu->plateau[5][2] = J2;
  jeu->plateau[4][2] = J1;
  jeu->plateau[3][2] = J2;
  jeu->plateau[2][2] = J1;

  jeu->plateau[5][3] = J1;
  jeu->plateau[4][3] = J1;
  jeu->plateau[3][3] = J2;

  jeu->plateau[5][4] = J2;
  jeu->plateau[5][5] = J2;
  jeu->plateau[5][6] = J1; // plateau 2

  // plateau 2 :
  jeu->courant = jeu->j1;
  CU_ASSERT_EQUAL(evaluation(jeu), -10);
  jeu->courant = jeu->j2;
  CU_ASSERT_EQUAL(evaluation(jeu), 10);
}

static CU_TestInfo test_array_IA[] = {
    {"vérifie la valeur d'une case", test_valeurCase},
    {"vérifie la valeur associée à chaque case d'un plateau "
     "rempli avec 6 jetons",
     test_autourP1},
    {"vérifie le score des joueurs pour un plateau "
     "rempli avec 6 jetons",
     test_scoreJoueurP1},
    {"vérifie les évaluation de 2 plateaux donnés, un "
     "rempli avec 6 jetons et l'autre avec 11",
     test_evaluation},
    {"vérifie la valeur associée à chaque case d'un plateau "
     "rempli avec 11 jetons",
     test_autourP2},
    {"vérifie le score des joueurs pour un plateau "
     "rempli avec 11 jetons",
     test_scoreJoueurP2},
    CU_TEST_INFO_NULL};

static CU_SuiteInfo suites[2] = {
    {"suiteScore", initSuite, cleanSuite, NULL, NULL, test_array_IA},
    CU_SUITE_INFO_NULL};

/**
 * @brief Get the Test IA Suites object
 *
 * @return CU_SuiteInfo* un tableau avec une suite de tests
 */
CU_SuiteInfo *getTestIASuites() { return suites; }