#include <CUnit/Basic.h>
#include 

int main(){
    if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    CU_ErrorCode error = CU_register_nsuites(1, getTestSuites());

    if(error != CUE_SUCCESS){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
}