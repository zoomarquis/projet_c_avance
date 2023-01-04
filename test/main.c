#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

//#include "test_p4.h"
#include "test_zoe.h"

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_ErrorCode error = CU_register_nsuites(1, getTestZoeSuites());

  /*  CU_ErrorCode error = CU_register_nsuites(
      1, (CU_SuiteInfo[2]){{" maSUite ", NULL, NULL, NULL, NULL, test_array},
                           CU_SUITE_INFO_NULL});
*/
  if (error != CUE_SUCCESS) {
    fprintf(stderr, "Problème: %s\n", CU_get_error_msg());
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return EXIT_SUCCESS;
}
