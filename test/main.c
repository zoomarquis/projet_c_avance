/**
 * @file main.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Le fichier main des tests unitaires.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_ia.h"
#include "test_p4.h"

/**
 * @brief Fonction princiale pour lancer les tests unitaires.
 *
 * @return int un code d'erreur ou de succès
 */
int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_ErrorCode error =
      CU_register_nsuites(2, getTestP4Suites(), getTestIASuites());

  if (error != CUE_SUCCESS) {
    fprintf(stderr, "Problème: %s\n", CU_get_error_msg());
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_get_run_summary();
  CU_cleanup_registry();

  return EXIT_SUCCESS;
}
