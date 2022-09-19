#include "s21_matrix.h"

int s21_determinant(matrix_t *A, float *result) {
    int res = 0;
    *result = 0;
    if (!(res = check_matrix_is_square(A))) {
        if (A->rows == 1) {
            *result = A->matrix[0][0];
        } else {
            matrix_t tmp = {0};
            if (!(res = s21_calc_complements(A, &tmp))) {
                for (int y = 0; y < A->columns; y++) {
                    *result += A->matrix[y][0] * tmp.matrix[y][0];
                }
            }
            s21_remove_matrix(&tmp);
        }
    } else {
        *result = NAN;
    }
    return res;
}
