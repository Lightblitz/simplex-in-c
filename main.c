#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "simplex.h"
#include "linalg.h"

#define FIELD_WIDTH 10

typedef struct {
    int rows, cols;
    double **A;
    double *b;
    double *c;
} InputData;

// Helper to draw a labeled box for input
void draw_input_grid(int start_y, int start_x, InputData *data) {
    int rows = data->rows;
    int cols = data->cols;

    mvprintw(start_y - 2, start_x, "Matrix A (Constraints) | Vector b (RHS)");
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Draw the bracket
            mvprintw(start_y + i, start_x + (j * FIELD_WIDTH), "[        ]");
            // Print the actual value inside the bracket
            mvprintw(start_y + i, start_x + (j * FIELD_WIDTH) + 1, "%7.2f", data->A[i][j]);
        }
        // Draw Vector b (RHS)
        mvprintw(start_y + i, start_x + (cols * FIELD_WIDTH) + 2, "| [        ]");
        mvprintw(start_y + i, start_x + (cols * FIELD_WIDTH) + 5, "%7.2f", data->b[i]);
    }

    mvprintw(start_y + rows + 1, start_x, "Vector c (Objective Function)");
    for (int j = 0; j < cols; j++) {
        mvprintw(start_y + rows + 2, start_x + (j * FIELD_WIDTH), "[        ]");
        mvprintw(start_y + rows + 2, start_x + (j * FIELD_WIDTH) + 1, "%7.2f", data->c[j]);
    }
}

void get_dimensions(int *r, int *c) {
    echo();
    curs_set(1);
    mvprintw(2, 2, "--- SIMPLEX CONFIGURATION ---");
    mvprintw(4, 2, "Enter number of constraints (m): ");
    scanw("%d", r);
    mvprintw(5, 2, "Enter number of variables   (n): ");
    scanw("%d", c);
    noecho();
}

void handle_data_entry(InputData *data) {
    int cur_r = 0, cur_c = 0;
    int start_y = 10, start_x = 5;
    bool entering = true;

    while (entering) {
        clear();
        mvprintw(1, 2, "USE ARROWS TO NAVIGATE, ENTER TO EDIT, 'S' TO SOLVE");
        draw_input_grid(start_y, start_x, data);

        // Determine cursor position based on whether we are in A, b, or c
        int target_y, target_x;
        if (cur_r < data->rows) {
            target_y = start_y + cur_r;
            target_x = (cur_c < data->cols) ? (start_x + (cur_c * FIELD_WIDTH) + 1) : (start_x + (data->cols * FIELD_WIDTH) + 4);
        } else {
            target_y = start_y + data->rows + 2;
            target_x = start_x + (cur_c * FIELD_WIDTH) + 1;
        }

        move(target_y, target_x);
        int ch = getch();

        switch (ch) {
            case KEY_UP:    if (cur_r > 0) cur_r--; break;
            case KEY_DOWN:  if (cur_r < data->rows) cur_r++; break;
            case KEY_LEFT:  if (cur_c > 0) cur_c--; break;
            case KEY_RIGHT: if (cur_c < ((cur_r < data->rows) ? data->cols : data->cols - 1)) cur_c++; break;
            case '\n':      // User wants to edit cell
                echo();
                double temp;
                mvprintw(target_y, target_x, "        "); // clear cell
                move(target_y, target_x);
                scanw("%lf", &temp);
                if (cur_r < data->rows) {
                    if (cur_c < data->cols) data->A[cur_r][cur_c] = temp;
                    else data->b[cur_r] = temp;
                } else {
                    data->c[cur_c] = temp;
                }
                noecho();
                break;
            case 's':
            case 'S':
                entering = false;
                break;
        }
    }
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    InputData id;

    // Phase 1: Setup
    get_dimensions(&id.rows, &id.cols);

    // Allocate memory
    id.A = (double **)malloc(id.rows * sizeof(double *));
    for (int i = 0; i < id.rows; i++) id.A[i] = (double *)calloc(id.cols, sizeof(double));
    id.b = (double *)calloc(id.rows, sizeof(double));
    id.c = (double *)calloc(id.cols, sizeof(double));

    // Phase 2: Data Entry
    handle_data_entry(&id);

    // Phase 3: Bridge to Backend
    clear();
    mvprintw(2, 2, "Solving...");
    refresh();
    
    // Create linalg matrix from input
    mat *matrixA = mat_new(id.rows, id.cols);
    for(int i=0; i<id.rows; i++)
        for(int j=0; j<id.cols; j++)
            matrixA->data[i][j] = id.A[i][j];

    // Note: solve_simplex needs to be updated to output to ncurses 
    // or you can endwin() before calling it to return to standard terminal.
    endwin(); 

    solve_simplex(matrixA, id.b, id.c);

    return 0;
}