#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int res = INCORRECT_MATRIX;
    *result = init_struct();
    if (rows > 0 && columns > 0) {
        result->matrix = set_matrix(columns, rows);
        if (result->matrix != NULL) {
            result->rows = rows;
            result->columns = columns;
            res = CORRECT_MATRIX;
        }
    }
    return res;
}
