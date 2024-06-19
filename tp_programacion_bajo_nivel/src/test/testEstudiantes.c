#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "estudiantes.h"

void test_createEstudiante(void) {
    Estudiante *est = createEstudiante("Juan", 20);
    CU_ASSERT_PTR_NOT_NULL(est);
    CU_ASSERT_STRING_EQUAL(est->nombre, "Juan");
    CU_ASSERT_EQUAL(est->edad, 20);

    free(est);
}

void test_agregarEstudiante(void) {
    LinkedList *lista = newLinkedList();
    
    agregarEstudiante(lista, "Ana", 22);
    CU_ASSERT_EQUAL(lista->size, 1);

    Estudiante *est = (Estudiante*)lista->head->value;
    CU_ASSERT_STRING_EQUAL(est->nombre, "Ana");
    CU_ASSERT_EQUAL(est->edad, 22);
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Suite_Estudiantes", 0, 0);

    CU_add_test(suite, "test_createEstudiante", test_createEstudiante);
    CU_add_test(suite, "test_agregarEstudiante", test_agregarEstudiante);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
