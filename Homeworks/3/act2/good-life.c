/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop
/// and have different Rule functions.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  syntax: there are a number of warnings that cause errors.
/// </li>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  design: the implementation needs function and value refactoring.
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

void header(void) /*function for program header*/
{
    printf("\n\t..Welcome to the Game of life..\n");
}

void survivalRule(char life[][20]) { // fix 1: removed the unused x and y
    int row, col;
    int neighbors = 0;
    for (row = 1; row < 19; row++) {
        for (col = 1; col < 19; col++) {
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

void birthRule(char life[][20]) { // fix 2: removed the unused x and y
    int row, col;
    int neighbors = 0;
    for (row = 1; row < 19; row++) {
        for (col = 1; col < 19; col++) {
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
    char life[20][20];
    int orgs; // fix 3: removed gens
    int i, row, col;
    int count = 0;
    /* int x = 19; */ // fix 4: removed unused x and y
    /* int y = 19; */

    header();

    printf("\nPlease enter the initial number of organisms: ");
    scanf("%i", &orgs);

    srand(31);

    for (i = 0; i < orgs; i++) {
        row = rand();
        row %= 20;
        col = rand();
        col %= 20;
        life[row][col] = '*';
    }

    for (row = 0; row < 20; row++) {
        for (col = 0; col < 20; col++) {
            if (life[row][col] != '*')
                life[row][col] = ' ';
        }
    }

    for (row = 0; row < 20; row++) {
        for (col = 0; col < 20; col++) {
            printf("%c", life[row][col]); // fix 1: changed %s to %c for char
        }
        puts(" ");
    }

    while (count < 100) { // changed limit to 100 generations (count)
        birthRule(life);
        survivalRule(life);
        for (row = 0; row < 20; row++) {
            for (col = 0; col < 20; col++) {
                printf("%c", life[row][col]);
            }
            puts(" ");
        }
        printf("\ngeneration: %d\n", count);
        count++;
    }

    return 0;
}
