#include "s21_matrix.h"

int s21_mult_number(matrix_t *A, float number, matrix_t *result) {
    int res = INCORRECT_MATRIX;
    *result = init_struct();
    if (check_matrix_is_correct(A) &&
        !(res = s21_create_matrix(A->rows, A->columns, result))) {
        for (int x = 0; x < A->rows; x++) {
            for (int y = 0; y < A->columns; y++) {
                result->matrix[x][y] = A->matrix[x][y] * number;
            }
        }
    }
    return res;
}
