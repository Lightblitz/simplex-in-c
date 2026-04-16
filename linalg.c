#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"

// struct mat_s stores the data for a matrix structure
typedef struct mat_s {
    int row_count;
    int col_count;
    double **data; // data[i][j] <-> array[i*m + j]
    int is_square;
} mat;

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// MATH HELPER FUNCTIONS
// ---------------------------------------------------------------
// ---------------------------------------------------------------

#define	RAND_MAX	0x7fffffff
static double nml_rand_interval(double min, double max) {
  double d;
  d = (double) rand() / ((double) RAND_MAX + 1);
  return (min + d * (max - min));
}
/*
double nml_exp(double base, const int exp) {
  
}
*/



// ---------------------------------------------------------------
// ---------------------------------------------------------------
// FOLLOWING FUNCTIONS ARE BASIC MATRIX ADT FUNCTIONS
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// mat_new(num_rows, num_cols) allocates memory to create a new matrix
mat *mat_new(int num_rows, int num_cols) {
    if (num_rows == 0) {
        MAT_ERROR("mat_new() called with num_rows = 0");
        return NULL;
    }
    if (num_cols == 0) {
        MAT_ERROR("mat_new() called with num_cols = 0");
        return NULL;
    }
    mat *m = malloc(sizeof(mat));
    NP_CHECK(m);
    m->row_count = num_rows;
    m->col_count = num_cols;
    m->is_square = (num_cols == num_rows) ? 1: 0;
    m->data = calloc(m->row_count, sizeof(*m->data));
    NP_CHECK(m->data);

    for (int i = 0; i < m->row_count; i++) {
        m->data[i] = calloc(m->col_count, sizeof(double));
        NP_CHECK(m->data[i]);
    }
    return m;
}

// mat_free takes a pointer to a matrix and frees the memory
void mat_free(mat *matrix) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < matrix->row_count; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

// mat_sqr takes a natural number and creates a square matrix with that dimension
mat *mat_sqr(int dimension) {
    if (dimension == 0) {
        MAT_ERROR("mat_sqr() called with dimension = 0");
        return NULL;
    }
    return mat_new(dimension, dimension);
}

// mat_rnd takes the dimensions of a matrix, along with a lower and upper bound
// and creates a matrix with entries generated randomly between the bounds
mat *mat_rnd(int num_rows, int num_cols, double min, double max) {
    if (num_rows == 0) {
        MAT_ERROR("mat_rnd() called with num_rows = 0");
        return NULL;
    }
    if (num_cols == 0) {
        MAT_ERROR("mat_rnd() called with num_cols = 0");
        return NULL;
    }
    mat *m = mat_new(num_rows, num_cols);
    for (int i = 0; i < m->row_count; i++) {
        for (int j = 0; j< m->col_count; j++) {
            m->data[i][j] = nml_rand_interval(min, max);// randomly generate from the interval [min, max]
        }
    }
    return m;
}

// mat_identity creates an identity matrix given a dimension
mat *mat_identity(int dimension) {
    if (dimension == 0) {
        MAT_ERROR("mat_identity() called with dimension = 0");
        return NULL;
    }
    mat *m = mat_new(dimension, dimension);
    for (int i = 0; i < dimension; i++) {
        m->data[i][i] = 1.0;
    }
    return m;
}

// mat_cp() copies the matrix m
mat *mat_cp(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
    }
    mat *new = mat_new(m->row_count, m->col_count);
    for (int i = 0; i < m->row_count; i++) {
        for (int j = 0; j < m->col_count; j++) {
            new->data[i][j] = m->data[i][j];
        }
    }
    return new;
}

bool mat_is_square(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
    }
    return (m->row_count == m->col_count);
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// PRINT FUNCTION PRINTS OUT A MATRIX
// ---------------------------------------------------------------
// ---------------------------------------------------------------

// mat_print prints out a matrix with each entry indicated in the format [i]
void mat_print(const mat *matrix) {
    if (matrix == NULL) {
        MAT_ERROR("mat_print() called with NULL matrix");
        return;
    }
    int rows = matrix->row_count;
    int cols = matrix->col_count;
    for (int i = 0; i < rows; i++) {
        //printf("| ");
        for (int j = 0; j < cols; j++) {
            printf("[%.2lf]", matrix->data[i][j]);
        }
        printf("\n");
    }
    return;
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// READ MATRIX FROM INPUT
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// read_mat() reads a matrix from standard input and creates a new matrix
mat *read_mat() {
    unsigned row, col;
    if (scanf("%u %u", &row, &col) == 2) {
        mat *m = mat_new(row, col);
        int counter = 0;
        int max = row * col;
        double val;
        while(counter < max) {
            if (scanf("%lf", &val) != 1) {
                mat_free(m);
                return NULL;
            }
            m->data[counter / col][counter % col] = val;
            counter++;
        }
        return m;
    }
    return NULL;
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// STANDARD MATRIX ARITHMETIC
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Note: Operations don't free memory. Must be done manually


// mat_add adds two matrices together and returns a pointer to the new matrix. Does not free memory
mat *mat_add(const mat *m1, const mat *m2) {
    if (m1 == NULL || m2 == NULL) {
        MAT_ERROR("mat_add() called with NULL pointers");
        return NULL;
    }
    if (m1->row_count == m2->row_count && m1->col_count == m2->col_count) {
        mat *dest = mat_new(m1->row_count, m1->col_count);
        for (int i = 0; i < m1->row_count; i++) {
            for (int j = 0; j < m1->col_count; j++) {
                dest->data[i][j] = m1->data[i][j] + m2->data[i][j]; 
            }
        }
        return dest;
    } else {
        MAT_ERROR("mat_add() called with matrices of different sizes");
        return NULL;
    }
}

// mat_sub subtracts two matrices and returns a pointer to the new matrix. Does not free memory
mat *mat_sub(const mat *m1, const mat *m2) {
    if (m1 == NULL || m2 == NULL) {
        MAT_ERROR("mat_sub() called with NULL pointers");
        return NULL;
    }
    if (m1->row_count == m2->row_count && m1->col_count == m2->col_count) {
        mat *dest = mat_new(m1->row_count, m1->col_count);
        for (int i = 0; i < m1->row_count; i++) {
            for (int j = 0; j < m1->col_count; j++) {
                dest->data[i][j] = m1->data[i][j] - m2->data[i][j]; 
            }
        }
        return dest;
    } else {
        MAT_ERROR("mat_sub() called with matrices of different sizes");
        return NULL;
    }
}

// mat_mult multiplies two matrices and returns a pointer to the new matrix. Does not free memory
mat *mat_mult(const mat *m1, const mat *m2) {
    if (m1 == NULL || m2 == NULL) {
        MAT_ERROR("mat_mult() called with NULL pointers");
        return NULL;
    }
    if (m1->col_count == m2->row_count) {
        mat *dest = mat_new(m1->row_count, m2->col_count);
        for (int i = 0; i < dest->row_count; i++) {
            for (int j = 0; j < dest->col_count; j++) {
                double sum = 0.0;
                for (int k = 0; k < m1->col_count; k++) {
                    sum += m1->data[i][k] * m2->data[k][j];
                }
                dest->data[i][j] = sum;
            }
        }
        return dest;
    } else {
        MAT_ERROR("mat_mult() called with invalid matrix sizes");
        return NULL;
    }
}

// mat_neg negates all elements in a matrix
mat *mat_neg(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("mat_neg() called with NULL pointers")
        return NULL;
    }
    mat *n = mat_new(m->row_count, m->col_count);
    for (int i = 0; i < m->row_count; i++) {
        for (int j = 0; j < m->col_count; j++) {
            n->data[i][j] = - m->data[i][j];
        }
    }
    return n;
}

// mat_transpose transposes a matrix
mat *mat_transpose(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("mat_transpose() called with NULL pointers")
        return NULL;
    }
    mat *t = mat_new(m->col_count, m->row_count);
    for (int i = 0; i < m->row_count; i++) {
        for (int j = 0; j < m->col_count; j++) {
            t->data[j][i] = m->data[i][j];
        }
    }
    return t;
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ELEMENTARY ROW OPERATIONS & COLUMN OPERATIONS
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// mat_ero_scalar() performs the ERO of scaling a row by a scalar
// Effects: Modifies the data stored in m
void mat_ero_scalar(const mat *m, unsigned row, double scalar) {
    if (m == NULL) {
        MAT_ERROR("mat_ero_scalar() called with NULL pointers");
        return;
    }
    if (row < 0) {
        MAT_ERROR("Function called with negative index");
    }
    for (int i = 0; i < m->col_count; i++) {
        m->data[row][i] *= scalar;
    }
}

// mat_ero_swap() performs the ERO of swapping row 1 and row 2
// Effects: Modifies the data stored in m
void mat_ero_swap(const mat *m, unsigned row1, unsigned row2) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
    }
    if (row1 < 0 || row2 < 0) {
        MAT_ERROR("Function called with negative index");
    }
    double *temp = m->data[row1];
    m->data[row1] = m->data[row2];
    m->data[row2] = temp;
}

// mat_ero_add() performs the ERO of adding scalar * row 2 to row 1
// Effects: Modifies the data stored in m
void mat_ero_add(const mat *m, unsigned row1, unsigned row2, double scalar) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
    }
    for (int j = 0; j < m->col_count; j++) {
        m->data[row1][j] += scalar * m->data[row2][j];
    }
}

// det_ero_scalar() performs the ERO of scaling a row by a scalar
// Effects: Modifies the data stored in m
static double det_ero_scalar(const mat *m, unsigned row, double scalar) {
    if (m == NULL) {
        MAT_ERROR("mat_ero_scalar() called with NULL pointers");
        return 0;
    }
    if (row < 0) {
        MAT_ERROR("Function called with negative index");
    }
    for (int i = 0; i < m->col_count; i++) {
        m->data[row][i] *= scalar;
    }
    return scalar;
}

// det_ero_swap() performs the ERO of swapping row 1 and row 2
// Effects: Modifies the data stored in m
static int det_ero_swap(const mat *m, unsigned row1, unsigned row2) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return 0;
    }
    if (row1 < 0 || row2 < 0) {
        MAT_ERROR("Function called with negative index");
    }
    double *temp = m->data[row1];
    m->data[row1] = m->data[row2];
    m->data[row2] = temp;
    if (row1 == row2) {
        return 1;
    } else {
        return -1;
    }
}

// det_ero_add() performs the ERO of adding scalar * row 2 to row 1
// Effects: Modifies the data stored in m
static int det_ero_add(const mat *m, unsigned row1, unsigned row2, double scalar) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return 0;
    }
    for (int j = 0; j < m->col_count; j++) {
        m->data[row1][j] += scalar * m->data[row2][j];
    }
    return 1;
}


// ---------------------------------------------------------------
// ---------------------------------------------------------------
// FUNCTIONS LEADING TO RREF
// ---------------------------------------------------------------
// ---------------------------------------------------------------

// pivot_finder() returns the index of the first row with a non-zero entry given the column index
// The row argument tells the function which row to start searching from.
// Function returns -1 if there is no pivot (i.e. the matrix is degenerate)
// Example: pivot_finder(m, 1, 1) says to search for the first non-zero entry in the second column
// starting from the second row
static int pivot_finder(const mat *m, unsigned col, unsigned row) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return -1;
    }
    
    for (int j = row; j < m->row_count; j++) {
        if (m->data[j][col] != 0) {
            return j;
        }
    }
    return -1;
}


// mat_ref() reduces matrix m into reduced echelon form
mat *mat_ref(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return NULL;
    }
    mat *ref = mat_cp(m);
    int row_ind = 0, col_ind = 0;
    int pivot;
    while (row_ind < m->row_count && col_ind < m->col_count) {
        pivot = pivot_finder(ref, col_ind, row_ind);
        
        // If column is 0, move to next column and skip below
        if (pivot < 0) {
            col_ind++;
            continue;
        }

        // Swap the pivot row w/ current row
        mat_ero_swap(ref, pivot, row_ind);

        // Otherwise reduce the leading coefficient of the pivot to 1
        mat_ero_scalar(ref, row_ind, 1/ref->data[row_ind][col_ind]);
        
        // Ensure all numbers below the pivot are zero for REF
        for (int k = row_ind + 1; k < ref->row_count; k++) {
            mat_ero_add(ref, k, row_ind, -(ref->data[k][col_ind]));
        }

        // Iterate to next column and move down one row
        row_ind++;
        col_ind++;
    }
    return ref;
}

// mat_rref() finds the reduced row echelon form of a matrix m
mat *mat_rref(const mat *m) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return NULL;
    }
    mat *rref = mat_ref(m);
    unsigned row_ind = 0, col_ind = 0;
    while (row_ind < rref->row_count && col_ind < rref->col_count) {
        // printf("%u %u \n", row_ind, col_ind);
        // If leading one position is a zero, shift to next column
        if (rref->data[row_ind][col_ind] == 0) {
            col_ind++;
            // printf("Skipped Column\n");
            continue;
        }
        // Otherwise, row_ind col_ind defines a leading one
        // Clear the items above the leading one to zero.
        for (int k = row_ind - 1; k > -1; k--) {
            // printf("Added %.2f * row index %d to row %d \n", -(rref->data[k][col_ind]), row_ind, k);
            mat_ero_add(rref, k, row_ind, -(rref->data[k][col_ind]));
        }
        col_ind++;
        row_ind++;
    }
    return rref;
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// USING REF TO FIND DETERMINANT
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// mat_determinant() finds the determinant by multiplying the determinants of the elementary matrices
// and the resulting REF matrix.
// Correction: This function determines the determinant by row reducing until upper triangular
double mat_determinant (const mat *m) {
    if (m == NULL) {
        MAT_ERROR("Function called with NULL pointer");
        return 0;
    }
    if (!mat_is_square(m)) {
        MAT_ERROR("Function called with non-square matrix");
        return 0;
    }
    mat *ref = mat_cp(m);
    double det = 1;
    int row_ind = 0, col_ind = 0;
    int pivot;
    while (row_ind < m->row_count && col_ind < m->col_count) {
        pivot = pivot_finder(ref, col_ind, row_ind);
        
        // If column is 0, then matrix is not invertible
        if (pivot < 0) {
            mat_free(ref);
            return 0;
        }

        // Otherwise reduce the leading coefficient of the pivot to 1
        // det *= det_ero_scalar(ref, pivot, 1/ref->data[pivot][col_ind]);

        // Swap the pivot row w/ current row
        if (pivot != row_ind) {
            det *= det_ero_swap(ref, pivot, row_ind);
        }
        
        // Ensure all numbers below the pivot are zero for REF
        for (int k = row_ind + 1; k < ref->row_count; k++) {
            det_ero_add(ref, k, row_ind, -(ref->data[k][col_ind] / ref->data[row_ind][col_ind]));
        }

        // Iterate to next column and move down one row
        row_ind++;
        col_ind++;
    }

    // multiply along the diagonal of the ref 
    for (int i = 0; i < ref->row_count; i++) { 
        det *= ref->data[i][i]; 
    }

    mat_free(ref);
    return det;
}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// INVERSE
// ---------------------------------------------------------------
// ---------------------------------------------------------------


// Rewrite the second while loop into the first part for a single passthrough
// i.e., subtract pivot row * coeff from the rows above the current row index
// mat_inverse() finds the inverse of the matrix using EROs
mat *mat_inverse(mat *m) {
    if (m->row_count != m->col_count) {
        MAT_ERROR("Calling a function for square matrices with a non-square matrix");
        return NULL;
    }
    mat *inv = mat_identity(m->row_count);
    mat *copy = mat_cp(m);
    // Now we implement an RREF function on the copied matrix, and 
    // apply the corresponding row operations onto the inverse
    // Return the pointer to the inverse matrix.
    int row_ind = 0, col_ind = 0;
    int pivot;
    while (row_ind < m->row_count && col_ind < m->col_count) {
        pivot = pivot_finder(copy, col_ind, row_ind);
        
        // If column is 0, move to next column and skip below
        if (pivot < 0) {
            MAT_ERROR("Matrix is not invertible");
            return NULL;
        }

        // Swap the pivot row w/ current row
        mat_ero_swap(copy, pivot, row_ind);
        mat_ero_swap(inv, pivot, row_ind);
        
        // Otherwise reduce the leading coefficient of the pivot to 1
        double scalar = 1/copy->data[row_ind][col_ind];
        mat_ero_scalar(copy, row_ind, scalar);
        mat_ero_scalar(inv, row_ind, scalar);

        // Ensure all numbers below the pivot are zero for REF
        for (int k = row_ind + 1; k < copy->row_count; k++) {
            mat_ero_add(copy, k, row_ind, -(copy->data[k][col_ind]));
            mat_ero_add(inv, k, row_ind, -(inv->data[k][col_ind]));
        }

        // Iterate to next column and move down one row
        row_ind++;
        col_ind++;
    }

    // While loop to reduce REF to RREF
    unsigned row_ind_2 = 0, col_ind_2 = 0;
    while (row_ind_2 < copy->row_count && col_ind_2 < copy->col_count) {
        // printf("%u %u \n", row_ind, col_ind);
        // If leading one position is a zero, shift to next column
        if (copy->data[row_ind_2][col_ind_2] == 0) {
            col_ind_2++;
            // printf("Skipped Column\n");
            continue;
        }
        // Otherwise, row_ind col_ind defines a leading one
        // Clear the items above the leading one to zero.
        for (int k = row_ind_2 - 1; k > -1; k--) {
            // printf("Added %.2f * row index %d to row %d \n", -(copy->data[k][col_ind_2]), row_ind_2, k);
            mat_ero_add(copy, k, row_ind_2, -(copy->data[k][col_ind_2]));
            mat_ero_add(inv, k, row_ind_2, -(inv->data[k][col_ind_2]));
        }
        col_ind_2++;
        row_ind_2++;
    }
    mat_free(copy);
    return inv;

}

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// EIGENVALUES AND EIGENVECTORS
// ---------------------------------------------------------------
// ---------------------------------------------------------------

double *mat_eigenvalues(mat *m) {
    return NULL;
}

mat *mat_eigenvectors(mat *m) {
    return NULL;
}

