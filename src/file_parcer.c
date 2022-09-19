#include "file_parcer.h"

#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream);

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF) {
        size_t len = p - bufptr;
        if (len > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            p = bufptr + len;
        }
        *p = c;
        p++;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

Model_data initModelDataStruct() {
    Model_data res = {0, 0, INFINITY, -INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY, {0}, NULL};
    return res;
}

Model_data parse_file(char const *file_name) {
    FILE *file = NULL;
    Model_data model = initModelDataStruct();
    if ((file = fopen(file_name, "r")) == NULL) {
        printf("No such file or directory: %s\n", file_name);
    } else {
        parse_file_1(file, &model);
        rewind(file);
        parse_file_2(file, &model);
    }

    return model;
}

void print_facets(Model_data model) {
    for (unsigned i = 0; i < model.count_of_facets; i++) {
        for (unsigned k = 0; k < model.facets[i].numbers_of_vertexes_in_facets; k++) {
            printf("%u ", model.facets[i].vertexes[k]);
        }
        printf("\n");
    }
}

int is_digit(char a) {
    return a >= '0' && a <= '9';
}

void parse_facet(char *str, Model_data *model, int i) {
    char *start = str;

    while (*str && *str != '\n') {
        if (is_digit(*str)) {
            strtod(str, &str);
            model->facets[i].numbers_of_vertexes_in_facets++;
            while (*str != ' ' && *str != '\n') str++;
        } else {
            str++;
        }
    }

    model->facets[i].vertexes = calloc(model->facets[i].numbers_of_vertexes_in_facets, sizeof(unsigned int));

    str = start;
    int k = 0;
    while (*str && *str != '\n') {
        if (is_digit(*str)) {
            model->facets[i].vertexes[k++] = (unsigned int)strtod(str, &str);
            while (*str != ' ' && *str != '\n') str++;
        } else {
            str++;
        }
    }
}

int parse_file_2(FILE *file, Model_data *model) {
    int error = 0;
    error = s21_create_matrix(model->count_of_vertexes + 1, 3, &model->vertexes);
    model->facets = calloc(model->count_of_facets, sizeof(polygon_t));
    char *tmpline = NULL;
    size_t size = 0;
    int vertex_i = 1;
    int i = 0;
    while ((int)getline(&tmpline, &size, file) != -1) {
        if (!strncmp(tmpline, "v ", (size_t)2)) {
            char *str = tmpline;
            str++;
            for (int j = 0; j < 3; j++) {
                model->vertexes.matrix[vertex_i][j] = (float)strtod(str, &str);
                if (j == 0) {
                    if (model->x_min > model->vertexes.matrix[vertex_i][j])
                        model->x_min = model->vertexes.matrix[vertex_i][j];
                    if (model->x_max < model->vertexes.matrix[vertex_i][j])
                        model->x_max = model->vertexes.matrix[vertex_i][j];
                } else if (j == 1) {
                    if (model->y_min > model->vertexes.matrix[vertex_i][j])
                        model->y_min = model->vertexes.matrix[vertex_i][j];
                    if (model->y_max < model->vertexes.matrix[vertex_i][j])
                        model->y_max = model->vertexes.matrix[vertex_i][j];
                } else if (j == 2) {
                    if (model->z_min > model->vertexes.matrix[vertex_i][j])
                        model->z_min = model->vertexes.matrix[vertex_i][j];
                    if (model->z_max < model->vertexes.matrix[vertex_i][j])
                        model->z_max = model->vertexes.matrix[vertex_i][j];
                }
            }
            vertex_i++;
        }
        if (!strncmp(tmpline, "f ", (size_t)2)) {
            char *str = tmpline;
            str++;
            parse_facet(str, model, i++);
        }
    }
    free(tmpline);
    return error;
}

int parse_file_1(FILE *file, Model_data *model) {
    int error = 0;
    char *tmpline = NULL;
    size_t size = 0;
    while ((int)getline(&tmpline, &size, file) != -1) {
        if (!strncmp(tmpline, "v ", (size_t)2)) {
            model->count_of_vertexes++;
        }
        if (!strncmp(tmpline, "f ", (size_t)2)) {
            model->count_of_facets++;
        }
    }
    free(tmpline);
    return error;
}

void remove_model_data(Model_data *model) {
    s21_remove_matrix(&model->vertexes);
    for (unsigned i = 0; i < model->count_of_facets; ++i) {
        if (model->facets[i].vertexes) {
            free(model->facets[i].vertexes);
            model->facets[i].vertexes = NULL;
        }
    }
    if (model->facets) {
        free(model->facets);
        model->facets = NULL;
    }
    Model_data tmp = {0};
    *model = tmp;
}
