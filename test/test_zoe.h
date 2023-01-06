/**
 * @file test_zoe.h
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

#ifndef TEST_ZOE_H
/**
 * @def TEST_ZOE_H
 * @brief la garde
 */
#define TEST_ZOE_H
#include <CUnit/Basic.h>
CU_SuiteInfo *getTestZoeSuites();
int initSuite();
int cleanSuite();
#endif