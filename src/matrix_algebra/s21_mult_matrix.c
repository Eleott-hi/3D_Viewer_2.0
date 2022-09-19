#include "s21_matrix.h"

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    *result = init_struct();
    if (!(res = check_matrices_can_mul(A, B)) &&
        !(res = s21_create_matrix(A->rows, B->columns, result))) {
        for (int x = 0; x < A->rows; x++)
            for (int y = 0; y < B->columns; y++)
                for (int k = 0; k < A->columns; k++) {
                    result->matrix[x][y] += A->matrix[x][k] * B->matrix[k][y];
                }
    }
    return res;
}
