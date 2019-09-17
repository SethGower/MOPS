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

void survivalRule(char life[][GRID_SIZE]) { // fix 1: removed the unused x and y
    int row, col;
    int neighbors = 0;
    for (row = 1; row < GRID_SIZE - 1; row++) {
        for (col = 1; col < GRID_SIZE - 1; col++) {
            if (strcmp(&life[row][col], "*")) {
                // fix 2: bad comparison. Changed to strcmp
                if (life[row - 1][col - 1] == '*')
                    ++neighbors;
                if (life[row - 1][col] == '*')
                    ++neighbors;
                if (life[row - 1][col + 1] == '*')
                    ++neighbors;
                if (life[row][col - 1] == '*')
                    ++neighbors;
                if (life[row][col + 1] == '*')
                    ++neighbors;
                if (life[row + 1][col - 1] == '*')
                    ++neighbors;
                if (life[row + 1][col] == '*')
                    ++neighbors;
                if (life[row + 1][col + 1] == '*')
                    ++neighbors;
                if (neighbors == 2 || neighbors == 3) {
                    life[row][col] = '*'; // fix 2: was using conditional test
                }
            }
        }
    }
    return;
}

void birthRule(char life[][GRID_SIZE]) { // fix 2: removed the unused x and y
    int row, col;
    int neighbors = 0;
    for (row = 1; row < GRID_SIZE - 1; row++) {
        for (col = 1; col < GRID_SIZE - 1; col++) {
            if (strcmp(&life[row][col], " ")) {
                // fix 2: bad comparison. Changed to strcmp
                if (life[row - 1][col - 1] == '*')
                    neighbors++;
                if (life[row - 1][col] == '*')
                    neighbors++;
                if (life[row - 1][col + 1] == '*')
                    neighbors++;
                if (life[row][col - 1] == '*')
                    neighbors++;
                if (life[row][col + 1] == '*')
                    neighbors++;
                if (life[row + 1][col - 1] == '*')
                    neighbors++;
                if (life[row + 1][col] == '*')
                    neighbors++;
                if (life[row + 1][col + 1] == '*')
                    neighbors++;
                if (neighbors == 3) {
                    life[row][col] = '*'; // fix 2: was using conditional test
                }
            }
        }
    }

    return;
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

    srand(31);

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
            printf("%c", life[row][col]); // fix 1: changed %s to %c for char
        }
        puts(" ");
    }

    while (count < GEN_LIMIT) { // changed limit to GEN_LIMIT generations
        birthRule(life);
        survivalRule(life);
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
