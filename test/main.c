#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

//#include "test_p4.h"
#include "test_ia.h"
#include "test_zoe.h"

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_ErrorCode error =
      CU_register_nsuites(2, getTestZoeSuites(), getTestIASuites());

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
