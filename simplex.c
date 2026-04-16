#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linalg.h"

#define EPSILON 1e-9

// --- Helper: Find the Entering Column ---
// Returns the index of the most negative coefficient in the objective row.
int find_entering_column(mat *tableau) {
    int pivot_col = -1;
    double min_val = -EPSILON;
    int last_row = tableau->row_count - 1;

    // We check all columns except the last one (RHS)
    for (int j = 0; j < tableau->col_count - 1; j++) {
        if (tableau->data[last_row][j] < min_val) {
            min_val = tableau->data[last_row][j];
            pivot_col = j;
        }
    }
    return pivot_col;
}

// --- Helper: Find the Leaving Row (Ratio Test) ---
// Returns the index of the row with the minimum positive ratio.
int find_leaving_row(mat *tableau, int pivot_col) {
    int pivot_row = -1;
    double min_ratio = -1;
    int last_row = tableau->row_count - 1;
    int last_col = tableau->col_count - 1;

    for (int i = 0; i < last_row; i++) {
        double val = tableau->data[i][pivot_col];
        if (val > EPSILON) {
            double ratio = tableau->data[i][last_col] / val;
            if (pivot_row == -1 || ratio < min_ratio) {
                min_ratio = ratio;
                pivot_row = i;
            }
        }
    }
    return pivot_row;
}

// --- Core: Pivot Operation ---
// Uses the library's ERO functions to clear the pivot column.
void perform_pivot(mat *tableau, int p_row, int p_col) {
    // 1. Scale pivot row so the pivot element becomes 1.0
    double pivot_entry = tableau->data[p_row][p_col];
    mat_ero_scalar(tableau, p_row, 1.0 / pivot_entry);

    // 2. Eliminate entries in all other rows
    for (int i = 0; i < tableau->row_count; i++) {
        if (i != p_row) {
            double factor = -tableau->data[i][p_col];
            mat_ero_add(tableau, i, p_row, factor);
        }
    }
}

void solve_simplex(mat *A, double *b, double *c) {
    int rows = A->row_count;
    int cols = A->col_count;

    // Create Tableau: (constraints + 1) x (vars + slacks + 1)
    mat *t = mat_new(rows + 1, cols + rows + 1);

    // Fill Matrix A and Slacks
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            t->data[i][j] = A->data[i][j];
        }
        t->data[i][cols + i] = 1.0;          
        t->data[i][t->col_count - 1] = b[i]; 
    }

    // Fill Objective Function
    for (int j = 0; j < cols; j++) {
        t->data[rows][j] = -c[j]; 
    }

    // Algorithm Loop
    while (true) {
        int p_col = find_entering_column(t);
        if (p_col == -1) break; 

        int p_row = find_leaving_row(t, p_col);
        if (p_row == -1) {
            printf("Problem is unbounded.\n");
            mat_free(t);
            return;
        }

        perform_pivot(t, p_row, p_col);
    }

    // --- NEW: Extracting the Optimal Vector ---
    printf("\nOptimization Complete.\n");
    printf("Max Z = %.4f\n", t->data[rows][t->col_count - 1]);
    printf("Optimal Vector (x):\n");

    for (int j = 0; j < cols; j++) {
        int one_index = -1;
        bool is_basic = true;
        int ones_count = 0;

        // Check if column j is a basic column (part of the identity)
        for (int i = 0; i < t->row_count; i++) {
            if (t->data[i][j] > EPSILON || t->data[i][j] < -EPSILON) {
                if (t->data[i][j] > 1.0 - EPSILON && t->data[i][j] < 1.0 + EPSILON) {
                    ones_count++;
                    one_index = i;
                } else {
                    is_basic = false;
                    break;
                }
            }
        }

        // A basic variable must have exactly one '1' and all other entries '0'
        // Also, the '1' shouldn't be in the objective row (last row) for decision vars
        if (is_basic && ones_count == 1 && one_index < rows) {
            printf("  x[%d] = %.4f\n", j, t->data[one_index][t->col_count - 1]);
        } else {
            printf("  x[%d] = 0.0000\n", j);
        }
    }
    
    mat_free(t);
}