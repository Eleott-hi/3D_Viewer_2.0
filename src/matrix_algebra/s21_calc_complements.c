#include "s21_matrix.h"

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int res = 0;
    *result = init_struct();
    if (!(res = check_matrix_is_square(A)) &&
        !(res = s21_create_matrix(A->rows, A->columns, result))) {
        if (A->rows == 1) {
            result->matrix[0][0] = 1.;
        } else {
            matrix_t tmp;
            float debilism = 0;
            if (!(res = s21_create_matrix(A->rows - 1, A->columns - 1, &tmp))) {
                for (int x = 0; x < A->rows; x++) {
                    for (int y = 0; y < A->columns; y++) {
                        get_minor(&tmp, A, x, y);
                        s21_determinant(&tmp, &debilism);
                        result->matrix[x][y] = debilism * (((x + y) % 2) ? -1 : 1);
                    }
                }
            }
            s21_remove_matrix(&tmp);
        }
    }
    return res;
}
