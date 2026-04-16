#ifndef LINALG_H
#define LINALG_H

#include <stddef.h>  // for size_t

typedef struct mat_s {
    int row_count;
    int col_count;
    double **data; // data[i][j]
    int is_square;
} mat;

// MATRIX CREATION AND DESTRUCTION
mat *mat_new(int num_rows, int num_cols);
void mat_free(mat *matrix);
mat *mat_sqr(int dimension);
mat *mat_rnd(int num_rows, int num_cols, double min, double max);
mat *mat_identity(int dimension);
mat *mat_cp(const mat *m);
bool mat_is_square(const mat *m);

// PRINT FUNCTION
void mat_print(const mat *matrix);

// READ MATRIX
mat *read_mat();

// ARITHMETIC OPERATIONS ON MATRICES
mat *mat_add(const mat *m1, const mat *m2);
mat *mat_sub(const mat *m1, const mat *m2);
mat *mat_mult(const mat *m1, const mat *m2);
mat *mat_neg(const mat *m);
mat *mat_transpose(const mat *m);

// ELEMENTARY ROW OPERATIONS & COLUMN OPERATIONS
void mat_ero_scalar(const mat *m, unsigned row, double scalar);
void mat_ero_swap(const mat *m, unsigned row1, unsigned row2);
void mat_ero_add(const mat *m, unsigned row1, unsigned row2, double scalar);


// RREF FUNCTIONS
mat *mat_ref(const mat *m);
mat *mat_rref(const mat *m);

// DETERMINANT
double mat_determinant(const mat *m);

#endif // LINALG_H