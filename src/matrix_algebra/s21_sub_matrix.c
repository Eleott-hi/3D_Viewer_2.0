#include "s21_matrix.h"

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int res = 0;
    negate_matrix(B);
    res = s21_sum_matrix(A, B, result);
    negate_matrix(B);
    return res;
}

void negate_matrix(matrix_t *s_matrix) {
    for (int x = 0; x < s_matrix->rows; x++) {
        for (int y = 0; y < s_matrix->columns; y++) {
            s_matrix->matrix[x][y] = -s_matrix->matrix[x][y];
        }
    }
}
