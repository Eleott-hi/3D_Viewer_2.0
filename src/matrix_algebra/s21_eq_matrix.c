#include "s21_matrix.h"

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int res = SUCCESS;
    if (!check_matrices_can_sum(A, B)) {
        for (int x = 0; x < A->rows; x++) {
            for (int y = 0; y < A->columns; y++) {
                if (fabs(A->matrix[x][y] - B->matrix[x][y]) > EPSILON) {
                    res = FAILURE;
                    break;
                }
            }
            if (res == FAILURE) break;
        }
    } else {
        res = FAILURE;
    }
    return res;
}
