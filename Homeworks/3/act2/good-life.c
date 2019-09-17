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

#define GRID_SIZE 20
#define GEN_LIMIT 100
void header(void) /*function for program header*/
{
    printf("\n\t..Welcome to the Game of life..\n");
}

// fix 1 removed unused parameter

void transition(const int size, char life[][size]) {
    char nextLife[size][size];
    int row, col, neighbors = 0;

    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            neighbors = 0;
            if (0 != row && 0 != col && size != row && size != col) {
                if (0 != row && 0 != col)
                    if (life[row - 1][col - 1] == '*')
                        neighbors++;
                if (0 != row)
                    if (life[row - 1][col] == '*')
                        neighbors++;
                if (0 != row && size != col)
                    if (life[row - 1][col + 1] == '*')
                        neighbors++;
                if (0 != col)
                    if (life[row][col - 1] == '*')
                        neighbors++;
                if (size != col)
                    if (life[row][col + 1] == '*')
                        neighbors++;
                if (size != row && 0 != col)
                    if (life[row + 1][col - 1] == '*')
                        neighbors++;
                if (size != row)
                    if (life[row + 1][col] == '*')
                        neighbors++;
                if (size != row && size != col)
                    if (life[row + 1][col + 1] == '*')
                        neighbors++;
            }

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
    /* int x = GRID_SIZE -1; */ // fix 4: removed unused x and y
    /* int y = GRID_SIZE -1; */

    header();

    printf("\nPlease enter the initial number of organisms: ");
    scanf("%i", &orgs);

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

    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            printf("%c",
                   life[row][col]); // fix 1: changed %s to %c for char
        }
        puts(" ");
    }

    while (count < GEN_LIMIT) { // changed limit to GEN_LIMIT generations
        transition(GRID_SIZE, life);
        for (row = 0; row < GRID_SIZE; row++) {
            for (col = 0; col < GRID_SIZE; col++) {
                printf("%c", life[row][col]);
            }
            puts(" ");
        }
        printf("\ngeneration: %d\n", count);
        count++;
    }

    return 0;
}
