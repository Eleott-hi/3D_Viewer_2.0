#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/file_parcer.h"
#include "../src/matrix_algebra/s21_matrix.h"

double M_3x3[9] = {7.0, 15.0, 2.0, 3.25, 4.5, 9.0, -7.2, 0.65, 0.0};
double M_3x3_inverse[9] = {2.0, 5.0, 7.0, 6.0, 3.0, 4.0, 5.0, -2.0, -3.0};
double M_3x3_math[9] = {8.0, 2.0, -3.0, 0.000002, 4.47, 5.0, 0.3, 1.0, -6.0};
double M_3x3_zero[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
double M_3x1[3] = {4.2, 6.1, -5.0};
double M_3x2[6] = {5.2, -9.0, 8.5, 4.5689631, 4.25, -3.687};
double M_3x2_for_eq[6] = {5.2, -9.0, 8.5, 4.5689638, 4.25, -3.687};
double M_1[1] = {5.0};

matrix_t test_matrix(double *values, int rows, int columns) {
  matrix_t matrix;
  s21_create_matrix(rows, columns, &matrix);
  int counter = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      matrix.matrix[i][j] = values[counter];
      counter++;
    }
  }
  return matrix;
}

START_TEST(test_create) {
  matrix_t A;
  int test = s21_create_matrix(0, 0, &A);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(3, 0, &A);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(0, 2, &A);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(2, -1, &A);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(-4, 6, &A);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  test = s21_create_matrix(2, 12, &A);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  s21_remove_matrix(&A);

  test = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_equal) {
  matrix_t A;
  matrix_t B;

  s21_create_matrix(0, 0, &A);
  s21_create_matrix(0, 0, &B);
  int test = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(test, FAILURE);

  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  test = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(test, FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &B);
  test = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(test, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x2, 3, 2);
  B = test_matrix(M_3x2_for_eq, 3, 2);
  test = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(test, FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x2, 3, 2);
  B = test_matrix(M_3x2, 3, 2);
  test = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(test, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_create_matrix_test) {
  matrix_t A;
  int res = s21_create_matrix(1, 2, &A);
  ck_assert_int_eq(res, CORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_create_matrix_test_2) {
  matrix_t A = {0};
  int res = s21_create_matrix(1, -2, &A);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_create_matrix_test_3) {
  matrix_t A;
  int res = s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 0;
  A.matrix[0][2] = 0;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 1;
  A.matrix[1][2] = 0;
  A.matrix[2][0] = 0;
  A.matrix[2][1] = 0;
  A.matrix[2][2] = 1;
  ck_assert_int_eq(res, CORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_create_matrix_test_4) {
  matrix_t A;
  int res = s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 5;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 1;
  A.matrix[1][2] = 0;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 9;
  A.matrix[2][2] = 10;
  ck_assert_int_eq(res, CORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_eq_matrix_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;
  B.matrix[2][2] = 1;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_test_2) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(1, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_test_3) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 1;
  B.matrix[0][1] = 1;
  B.matrix[0][2] = 1;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 3;
  B.matrix[2][2] = 3;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_test_4) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  matrix_t B;
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 1.515673;
  B.matrix[0][0] = 1.515673;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_test_5) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  matrix_t B;
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 1.5156739;
  B.matrix[0][0] = 1.515673;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_test_6) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  matrix_t B;
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 1.515674;
  B.matrix[0][0] = 1.5156735;
  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_sum_and_sub_matrix_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;
  B.matrix[2][2] = 1;
  C.matrix[0][0] = 4;
  C.matrix[0][1] = 4;
  C.matrix[0][2] = 4;
  C.matrix[1][0] = 4;
  C.matrix[1][1] = 4;
  C.matrix[1][2] = 4;
  C.matrix[2][0] = 4;
  C.matrix[2][1] = 4;
  C.matrix[2][2] = 4;
  matrix_t my;
  s21_sum_matrix(&A, &B, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_sum_and_sub_matrix_test_2) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(1, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  matrix_t my;
  int result = s21_sum_matrix(&A, &B, &my);
  ck_assert_int_eq(result, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_sum_and_sub_matrix_test_3) {
  matrix_t A;
  s21_create_matrix(1, 3, &A);
  matrix_t B;
  s21_create_matrix(1, 3, &B);
  matrix_t C;
  s21_create_matrix(1, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  B.matrix[0][0] = 1;
  B.matrix[0][1] = 1;
  B.matrix[0][2] = 1;
  C.matrix[0][0] = 1;
  C.matrix[0][1] = 1;
  C.matrix[0][2] = 1;
  matrix_t my;
  s21_sum_matrix(&A, &B, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_sum_and_sub_matrix_test_4) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;
  B.matrix[2][2] = 1;
  C.matrix[0][0] = 4;
  C.matrix[0][1] = 4;
  C.matrix[0][2] = 4;
  C.matrix[1][0] = 4;
  C.matrix[1][1] = 4;
  C.matrix[1][2] = 4;
  C.matrix[2][0] = 4;
  C.matrix[2][1] = 4;
  C.matrix[2][2] = 4;
  matrix_t my;
  s21_sub_matrix(&A, &B, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_sum_and_sub_matrix_test_5) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(1, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  matrix_t my;
  int result = s21_sub_matrix(&A, &B, &my);
  ck_assert_int_eq(result, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_mult_number_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  double x = 2.0;
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  C.matrix[0][0] = 4;
  C.matrix[0][1] = 4;
  C.matrix[0][2] = 4;
  C.matrix[1][0] = 4;
  C.matrix[1][1] = 4;
  C.matrix[1][2] = 4;
  C.matrix[2][0] = 4;
  C.matrix[2][1] = 4;
  C.matrix[2][2] = 4;
  matrix_t my;
  s21_mult_number(&A, x, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_mult_number_test_2) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  double x = 0.0;
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  C.matrix[0][0] = 0;
  C.matrix[0][1] = 0;
  C.matrix[0][2] = 0;
  C.matrix[1][0] = 0;
  C.matrix[1][1] = 0;
  C.matrix[1][2] = 0;
  C.matrix[2][0] = 0;
  C.matrix[2][1] = 0;
  C.matrix[2][2] = 0;
  matrix_t my;
  s21_mult_number(&A, x, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_mult_matrix_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;
  B.matrix[2][2] = 1;
  C.matrix[0][0] = 3;
  C.matrix[0][1] = 3;
  C.matrix[0][2] = 3;
  C.matrix[1][0] = 4;
  C.matrix[1][1] = 4;
  C.matrix[1][2] = 4;
  C.matrix[2][0] = 3;
  C.matrix[2][1] = 3;
  C.matrix[2][2] = 32;
  matrix_t my;
  s21_mult_matrix(&A, &B, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_mult_matrix_test_2) {
  matrix_t A;
  s21_create_matrix(1, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 1;
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 2;
  B.matrix[1][2] = 2;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;
  B.matrix[2][2] = 1;
  matrix_t my;
  int result = s21_mult_matrix(&A, &B, &my);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_transpose_test) {
  matrix_t A;
  s21_create_matrix(3, 2, &A);
  matrix_t C;
  s21_create_matrix(2, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 6;
  C.matrix[0][0] = 1;
  C.matrix[0][1] = 4;
  C.matrix[0][2] = 2;
  C.matrix[1][0] = 3;
  C.matrix[1][1] = 5;
  C.matrix[1][2] = 6;
  matrix_t my;
  s21_transpose(&A, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_determinant_test) {
  // matrix_t A;
  // s21_create_matrix(3, 3, &A);
  // double det = 0;
  // A.matrix[0][0] = 1;
  // A.matrix[0][1] = 2;
  // A.matrix[0][2] = 3;
  // A.matrix[1][0] = 4;
  // A.matrix[1][1] = 5;
  // A.matrix[1][2] = 6;
  // A.matrix[2][0] = 7;
  // A.matrix[2][1] = 8;
  // A.matrix[2][2] = 9;
  // s21_determinant(&A, &det);
  // ck_assert_uint_eq(det, 0);
  // s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_determinant_test_2) {
  // matrix_t A;
  // s21_create_matrix(3, 2, &A);
  // A.matrix[0][0] = 1;
  // A.matrix[0][1] = 2;
  // A.matrix[1][0] = 4;
  // A.matrix[1][1] = 5;
  // A.matrix[2][0] = 7;
  // A.matrix[2][1] = 8;
  // double result;
  // s21_determinant(&A, &result);
  // ck_assert(result != result);
  // s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_calc_complements_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  C.matrix[0][0] = 0;
  C.matrix[0][1] = -10;
  C.matrix[0][2] = -20;
  C.matrix[1][0] = -4;
  C.matrix[1][1] = -14;
  C.matrix[1][2] = -8;
  C.matrix[2][0] = -8;
  C.matrix[2][1] = 2;
  C.matrix[2][2] = 4;
  matrix_t my;
  s21_calc_complements(&A, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_inverse_matrix_test) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t C;
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = -1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = -2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = -1;
  A.matrix[1][2] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = 4;
  C.matrix[0][0] = 0.6;
  C.matrix[0][1] = -0.4;
  C.matrix[0][2] = 0.8;
  C.matrix[1][0] = 0.7;
  C.matrix[1][1] = 0.2;
  C.matrix[1][2] = 0.1;
  C.matrix[2][0] = -0.1;
  C.matrix[2][1] = 0.4;
  C.matrix[2][2] = -0.3;
  matrix_t my;
  s21_inverse_matrix(&A, &my);
  int result = s21_eq_matrix(&my, &C);
  ck_assert_int_eq(result, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
  s21_remove_matrix(&my);
}
END_TEST

START_TEST(s21_calc_complement_test) {
  matrix_t A;
  s21_create_matrix(3, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  matrix_t res;
  ck_assert(s21_calc_complements(&A, &res) == INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(s21_calc_complement_test_2) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 1;
  matrix_t res;
  s21_calc_complements(&A, &res);
  ck_assert_int_eq(s21_eq_matrix(&A, &res), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(s21_calc_complement_test_3) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  matrix_t B;
  s21_create_matrix(3, 3, &B);
  matrix_t C;
  s21_create_matrix(1, 1, &C);
  matrix_t D;
  s21_create_matrix(1, 1, &D);

  D.matrix[0][0] = 1;
  C.matrix[0][0] = 5;

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  B.matrix[0][0] = 0;
  B.matrix[0][1] = 10;
  B.matrix[0][2] = -20;
  B.matrix[1][0] = 4;
  B.matrix[1][1] = -14;
  B.matrix[1][2] = 8;
  B.matrix[2][0] = -8;
  B.matrix[2][1] = -2;
  B.matrix[2][2] = 4;

  matrix_t RES;
  s21_calc_complements(&A, &RES);
  ck_assert_msg(s21_eq_matrix(&B, &RES), "TEST 1 FAIL");
  matrix_t RES1;
  s21_calc_complements(&C, &RES1);
  ck_assert_msg(s21_eq_matrix(&D, &RES1), "TEST 2 FAIL");
  s21_remove_matrix(&RES);
  s21_remove_matrix(&RES1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
  s21_remove_matrix(&D);
}
END_TEST

START_TEST(test_sum) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C;

  int test = s21_sum_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  s21_create_matrix(2, 3, &A);
  test = s21_sum_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&A);

  s21_create_matrix(2, 3, &B);
  test = s21_sum_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x3, 3, 3);
  B = test_matrix(M_3x2, 3, 2);
  test = s21_sum_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C;

  int test = s21_sub_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  s21_create_matrix(2, 3, &A);
  test = s21_sub_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&A);

  s21_create_matrix(2, 3, &B);
  test = s21_sub_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x3, 3, 3);
  B = test_matrix(M_3x2, 3, 2);
  test = s21_sub_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_number) {
  matrix_t A = {0};
  matrix_t B;

  int test = s21_mult_number(&A, 3, &B);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  A = test_matrix(M_3x1, 3, 1);
  test = s21_mult_number(&A, 5.0, &B);
  double result[3] = {21.0, 30.5, -25.0};
  matrix_t mul = test_matrix(result, 3, 1);
  int res = s21_eq_matrix(&mul, &B);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&mul);
}
END_TEST

START_TEST(test_mult) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C = {0};

  int test = s21_mult_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  s21_create_matrix(2, 3, &A);
  test = s21_mult_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&A);

  s21_create_matrix(2, 3, &B);
  test = s21_mult_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x2, 3, 2);
  B = test_matrix(M_3x2, 3, 2);
  test = s21_mult_matrix(&A, &B, &C);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_transpose) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C = {0};

  int test = s21_transpose(&A, &B);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  A = test_matrix(M_3x1, 3, 1);
  C = test_matrix(M_3x1, 1, 3);
  test = s21_transpose(&A, &B);
  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_calc_complements) {
  matrix_t A = {0};
  matrix_t B;

  int test = s21_calc_complements(&A, &B);
  ck_assert_int_eq(test, INCORRECT_MATRIX);

  s21_create_matrix(1, 3, &A);
  test = s21_calc_complements(&A, &B);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_1, 1, 1);
  test = s21_calc_complements(&A, &B);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  ck_assert_int_eq(B.matrix[0][0], 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_inverse_2) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t C = {0};

  int test = s21_inverse_matrix(&A, &B);
  ck_assert_int_eq(test, INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(1, 3, &A);
  test = s21_inverse_matrix(&A, &B);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x3_zero, 3, 3);
  test = s21_inverse_matrix(&A, &B);
  ck_assert_int_eq(test, INCORRECT_CALCULATE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_1, 1, 1);
  test = s21_inverse_matrix(&A, &B);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A = test_matrix(M_3x3_inverse, 3, 3);
  test = s21_inverse_matrix(&A, &B);
  double result[9] = {1.0, -1.0, 1.0, -38.0, 41.0, -34.0, 27.0, -29.0, 24.0};
  C = test_matrix(result, 3, 3);
  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(test, CORRECT_MATRIX);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_parse_2) {
  Model_data t = parse_file("cat.obj");
  remove_model_data(&t);
  ck_assert_int_eq(t.vertexes.columns, 0);
}
END_TEST

Suite *s21_str_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tcase_core = tcase_create("Equelity");

  tcase_add_test(tcase_core, test_create);
  tcase_add_test(tcase_core, test_equal);
  tcase_add_test(tcase_core, test_sum);
  tcase_add_test(tcase_core, test_sub);
  tcase_add_test(tcase_core, test_mult_number);
  tcase_add_test(tcase_core, test_mult);
  tcase_add_test(tcase_core, test_transpose);
  tcase_add_test(tcase_core, test_calc_complements);
  tcase_add_test(tcase_core, test_inverse_2);

  tcase_add_test(tcase_core, s21_calc_complement_test);
  tcase_add_test(tcase_core, s21_calc_complement_test_2);
  tcase_add_test(tcase_core, s21_calc_complement_test_3);
  tcase_add_test(tcase_core, s21_create_matrix_test);
  tcase_add_test(tcase_core, s21_create_matrix_test_2);
  tcase_add_test(tcase_core, s21_create_matrix_test_3);
  tcase_add_test(tcase_core, s21_create_matrix_test_4);
  tcase_add_test(tcase_core, s21_eq_matrix_test);
  tcase_add_test(tcase_core, s21_eq_matrix_test_2);
  tcase_add_test(tcase_core, s21_eq_matrix_test_3);
  tcase_add_test(tcase_core, s21_eq_matrix_test_4);
  tcase_add_test(tcase_core, s21_eq_matrix_test_5);
  tcase_add_test(tcase_core, s21_eq_matrix_test_6);
  tcase_add_test(tcase_core, s21_sum_and_sub_matrix_test);
  tcase_add_test(tcase_core, s21_sum_and_sub_matrix_test_2);
  tcase_add_test(tcase_core, s21_sum_and_sub_matrix_test_3);
  tcase_add_test(tcase_core, s21_sum_and_sub_matrix_test_4);
  tcase_add_test(tcase_core, s21_sum_and_sub_matrix_test_5);
  tcase_add_test(tcase_core, s21_mult_number_test);
  tcase_add_test(tcase_core, s21_mult_number_test_2);
  tcase_add_test(tcase_core, s21_mult_matrix_test);
  tcase_add_test(tcase_core, s21_mult_matrix_test_2);
  tcase_add_test(tcase_core, s21_transpose_test);
  tcase_add_test(tcase_core, s21_determinant_test);
  tcase_add_test(tcase_core, s21_determinant_test_2);
  tcase_add_test(tcase_core, s21_calc_complements_test);
  tcase_add_test(tcase_core, s21_inverse_matrix_test);
  tcase_add_test(tcase_core, test_parse_2);

  suite_add_tcase(suite, tcase_core);
  return suite;
}

int main(void) {
  Suite *suite = s21_str_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
