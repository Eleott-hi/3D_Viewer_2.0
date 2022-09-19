#include "s21_matrix.h"

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    *result = init_struct();
    if (!(res = check_matrices_can_sum(A, B)) &&
        !(res = s21_create_matrix(A->rows, A->columns, result))) {
        for (int x = 0; x < A->rows; x++) {
            for (int y = 0; y < A->columns; y++) {
                result->matrix[x][y] = A->matrix[x][y] + B->matrix[x][y];
            }
        }
    }
    return res;
}
