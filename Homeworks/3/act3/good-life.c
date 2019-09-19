/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop
/// and have different Rule functions.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  style: formatting is poor; the mix of TAB and spaces indentation
/// needs correction, and spacing between tokens is inconsistent.
/// The course style puts the '{' at the end of the function header, not
/// on a line of its own, and function headers belong at the left margin.
/// </li>
/// <li>  documentation: documentation is non-existent except for this block.
/// </li>
/// </ol>
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GRID_SIZE 20
#define GEN_LIMIT 100
void header(void) /*function for program header*/
{
    printf("\n\t..Welcome to the Game of life..\n");
}

/******************************************************************************
 * Function:         int mod
 *                   Simple function to allow modulo of negative nubmers
 * Where:
 *                   int a - Operand A
 *                   int b - Operand B
 * Return:           modulo of a%b
 * Error:            none
 *****************************************************************************/
int mod(const int a, const int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}
/******************************************************************************
 * Function:         int countNeighbors
 *                   Function to count neighbors of cell at coords row,col
 * Where:
 *                   int size - Size of the grid
 *                   char life[][size] - matrix for the game of life board
 *                   int row - row coordinate for cell to check
 *                   int col - column coordinate for cell to check
 * Return:           int neighborsNumber of neighbors to that cell (max 8)
 * Error:            None
 *****************************************************************************/
int countNeighbors(const int size, char life[][size], const int row,
                   const int col) {
    int neighbors = 0;
    int i, j;
    int curr_row, curr_col;
    /*
     * Loop to iterate through the 3x3 matrix around the cell in question
     */
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            /* check if cell is the one in question */
            if (0 == i && j == 0)
                continue;
            curr_row = mod(row + i, size);
            curr_col = mod(col + j, size);

            /* using the current row and column which is being checked, check
             * if it is a living cell, if it is increment neighbors */
            if (life[curr_row][curr_col] == '*')
                neighbors++;
        }
    }
    return neighbors;
}
// fix 1 removed unused parameter
/******************************************************************************
 * Function:         void transition
 *                   Apply the rules for whether if the cells live or die
 * Where:
 *                   const int size - size of the matrix
 *                   char life[][size] - matrix grid for Game of Life
 * Return:           N/A
 * Error:            None
 *****************************************************************************/
void transition(const int size, char life[][size]) {
    /* temporary variable for the new game of life */
    char nextLife[size][size];
    int row, col, neighbors = 0;

    /* iteerate through all of the grid */
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            neighbors = countNeighbors(size, life, row, col);
            /* applies actual rules for each cell in grid */
            if (life[row][col] == '*') {
                if (2 > neighbors || 3 < neighbors) {
                    nextLife[row][col] = ' ';
                } else {
                    nextLife[row][col] = '*';
                }
            } else {
                if (3 == neighbors)
                    nextLife[row][col] = '*';
                else
                    nextLife[row][col] = ' ';
            }
        }
    }
    /* iterates through the entire grid, and updates the old grid with new
     * grid */
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            life[row][col] = nextLife[row][col];
        }
    }
}

int main(void) {
    char life[GRID_SIZE][GRID_SIZE];
    int orgs; // fix 3: removed gens
    int i, row, col;
    int count = 0;

    header();

    printf("\nPlease enter the initial number of organisms: ");
    scanf("%i", &orgs);
    putchar('\n');

    srand(41);

    for (i = 0; i < orgs; i++) {
        row = rand();
        row %= GRID_SIZE;
        col = rand();
        col %= GRID_SIZE;
        life[row][col] = '*';
    }

    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (life[row][col] != '*')
                life[row][col] = ' ';
        }
    }

    printf("Initial Case\n");
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            putchar(life[row][col]); // fix 1: changed to putchar
        }
        putchar('\n');
    }

    while (count < GEN_LIMIT) { // changed limit to GEN_LIMIT generations
        transition(GRID_SIZE, life);
        printf("\ngeneration: %d\n", count);
        for (row = 0; row < GRID_SIZE; row++) {
            for (col = 0; col < GRID_SIZE; col++) {
                putchar(life[row][col]); // fix 1: changed to putchar
            }
            putchar('\n');
        }
        count++;
    }

    return 0;
}
