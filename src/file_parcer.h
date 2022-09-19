#pragma once

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_algebra/s21_matrix.h"

typedef struct Facets {
    unsigned int *vertexes;
    unsigned int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct Model_data {
    uint32_t count_of_vertexes;
    uint32_t count_of_facets;
    float x_min, x_max, y_min, y_max, z_min, z_max;
    matrix_t vertexes;
    polygon_t *facets;
} Model_data;

Model_data parse_file(char const *file_name);
int parse_file_1(FILE *file, Model_data *model);
int parse_file_2(FILE *file, Model_data *model);
void parse_facet(char *str, Model_data *model, int i);
void print_facets(Model_data model);
void remove_model_data(Model_data *model);
Model_data initModelDataStruct();
