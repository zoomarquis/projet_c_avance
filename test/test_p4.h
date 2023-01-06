/**
 * @file test_p4.h
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Définition des fonctions de tests unitaires du fichier
 * puissance_quatre.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TEST_P4_H
/**
 * @def TEST_P4_H
 * @brief la garde
 */
#define TEST_P4_H
#include <CUnit/Basic.h>
CU_SuiteInfo *getTestP4Suites();
int initSuite();
int cleanSuite();
#endif