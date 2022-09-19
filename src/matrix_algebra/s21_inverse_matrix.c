
#include "s21_matrix.h"

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
    int res = 0;
    float det = 0;
    matrix_t B = init_struct();
    matrix_t C = init_struct();
    *result = init_struct();
    if (!(res = s21_determinant(A, &det)) &&
        !(res = (det) ? CORRECT_MATRIX : INCORRECT_CALCULATE) &&
        !(res = s21_calc_complements(A, &B)) &&
        !(res = s21_transpose(&B, &C)) &&
        !(res = s21_mult_number(&C, 1. / det, result))) {
        res = CORRECT_MATRIX;
    }
    s21_remove_matrix(&B);
    s21_remove_matrix(&C);
    return res;
}
