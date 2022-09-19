#include "s21_matrix.h"

void print_matrix(matrix_t s_matrix) {
    for (int x = 0; x < s_matrix.rows; x++) {
        for (int y = 0; y < s_matrix.columns; y++) {
            printf("% 10g ", s_matrix.matrix[x][y]);
        }
        printf("\n");
    }
}

int check_matrix_is_correct(matrix_t *A) {
    return A && A->matrix &&
           A->columns > 0 && A->rows > 0;
}

int check_matrices_can_sum(matrix_t *A, matrix_t *B) {
    int res = INCORRECT_MATRIX;
    if (check_matrix_is_correct(A) && check_matrix_is_correct(B)) res = CORRECT_MATRIX;
    if (!res && !((A->columns == B->columns) && (A->rows == B->rows))) res = INCORRECT_CALCULATE;
    return res;
}

int check_matrices_can_mul(matrix_t *A, matrix_t *B) {
    int res = INCORRECT_MATRIX;
    if (check_matrix_is_correct(A) && check_matrix_is_correct(B)) res = CORRECT_MATRIX;
    if (!res && !(A->columns == B->rows)) res = INCORRECT_CALCULATE;
    return res;
}

int check_matrix_is_square(matrix_t *A) {
    int res = INCORRECT_MATRIX;
    if (check_matrix_is_correct(A)) res = CORRECT_MATRIX;
    if (!res && !(A->rows == A->columns)) res = INCORRECT_CALCULATE;
    return res;
}

void get_minor(matrix_t *minor, matrix_t *A, int i, int j) {
    int row = 0;
    for (int x = 0; x < A->rows; x++) {
        if (x != i) {
            int column = 0;
            for (int y = 0; y < A->columns; y++) {
                if (y != j) {
                    minor->matrix[row][column++] = A->matrix[x][y];
                }
            }
            row++;
        }
    }
}

matrix_t init_struct() {
    matrix_t res = {NULL, 0, 0};
    return res;
}

float **set_matrix(int x, int y) {
    float **arr = (float **)calloc(1, (y * sizeof(float *) + y * x * sizeof(float)));
    if (arr != NULL) {
        float *ptr = (float *)(arr + y);
        for (int i = 0; i < y; i++) {
            arr[i] = ptr + x * i;
        }
    }
    return arr;
}
