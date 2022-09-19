#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    CORRECT_MATRIX = 0,
    INCORRECT_MATRIX = 1,
    INCORRECT_CALCULATE = 2,
} matrix_type_t;

typedef struct matrix_struct {
    float **matrix;
    int rows;
    int columns;
} matrix_t;

#define EPSILON 1e-7
#define SUCCESS 1
#define FAILURE 0

// Функции по заданию

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_mult_number(matrix_t *A, float number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, float *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// Мои функции

float **set_matrix(int x, int y);
void print_matrix(matrix_t s_matrix);
matrix_t init_struct();
void negate_matrix(matrix_t *s_matrix);
int check_matrices_can_sum(matrix_t *A, matrix_t *B);
int check_matrices_can_mul(matrix_t *A, matrix_t *B);
int check_matrix_is_square(matrix_t *A);
int check_matrix_is_correct(matrix_t *A);
void get_minor(matrix_t *tmp, matrix_t *A, int i, int j);
matrix_t create_identity_matrix(int rows, int columns);
void check_type(matrix_t *A);

#endif  // SRC_S21_MATRIX_H_
