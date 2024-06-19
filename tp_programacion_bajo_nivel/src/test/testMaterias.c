#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "materias.h"

void test_createMateria(void) {
    Materia *mat = createMateria("Quimica");
    CU_ASSERT_PTR_NOT_NULL(mat);
    CU_ASSERT_STRING_EQUAL(mat->nombre, "Quimica");

    free(mat);
}

void test_agregarMateria(void) {
    LinkedList *lista = newLinkedList();

    agregarMateria(lista, "Biologia");
    CU_ASSERT_EQUAL(lista->size, 1);

    Materia *mat = (Materia*)lista->head->value;
    CU_ASSERT_STRING_EQUAL(mat->nombre, "Biologia");
}

int main(void) {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Suite_Materias", 0, 0);

    CU_add_test(suite, "test_createMateria", test_createMateria);
    CU_add_test(suite, "test_agregarMateria", test_agregarMateria);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
