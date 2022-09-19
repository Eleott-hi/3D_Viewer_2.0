#include "s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
    int res = INCORRECT_MATRIX;
    *result = init_struct();
    if (check_matrix_is_correct(A) &&
        !(res = s21_create_matrix(A->columns, A->rows, result))) {
        for (int x = 0; x < result->rows; x++)
            for (int y = 0; y < result->columns; y++)
                result->matrix[x][y] = A->matrix[y][x];
    }
    return res;
}
