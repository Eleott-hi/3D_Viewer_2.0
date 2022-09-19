#include "s21_matrix.h"

void s21_remove_matrix(matrix_t *A) {
    if (A->matrix) free(A->matrix);
    *A = init_struct();
}
