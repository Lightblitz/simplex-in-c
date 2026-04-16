#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "linalg.h"

/**
 * @brief Solves a standard linear programming maximization problem using the Simplex algorithm.
 * * @param A Pointer to a matrix representing the constraint coefficients (m x n).
 * @param b Array of size m representing the right-hand side of constraints (limits).
 * @param c Array of size n representing the coefficients of the objective function.
 */
void solve_simplex(mat *A, double *b, double *c);

/**
 * @brief Identifies the entering variable by finding the most negative coefficient 
 * in the objective row (the bottom row of the tableau).
 * * @param tableau The current simplex tableau.
 * @return The column index of the entering variable, or -1 if the solution is optimal.
 */
int find_entering_column(mat *tableau);

/**
 * @brief Identifies the leaving variable using the Minimum Ratio Test to maintain feasibility.
 * * @param tableau The current simplex tableau.
 * @param pivot_col The column index of the entering variable.
 * @return The row index of the leaving variable, or -1 if the problem is unbounded.
 */
int find_leaving_row(mat *tableau, int pivot_col);

/**
 * @brief Performs a pivot operation on the tableau to move to an adjacent basic feasible solution.
 * * @param tableau The simplex tableau to modify.
 * @param p_row The row index of the pivot element.
 * @param p_col The column index of the pivot element.
 */
void perform_pivot(mat *tableau, int p_row, int p_col);

#endif // SIMPLEX_H