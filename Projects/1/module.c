/******************************************************************************
 * File:             bracetopia.c
 *
 * Author:           Seth Gower
 * Created:          10/07/19
 * Description:      Module for Bracetopia for Project 1 for CSCI-243
 *****************************************************************************/

#include "ClassErrors.h"
#include "bracetopia.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/******************************************************************************
 * Function:         createBracetopia
 * Description:      Initializes the struct for the board
 * Where:
 *                   BracetopiaBoard *boardPtr - pointer to board
 *                   size_t size - square size of the board
 *                   double threshold - happiness threshold for program
 *                   double percVac - percentage of vacant spots in board
 *                   double percEnd - percentage of endline agents in board
 *                    -
 * Return:
 * Error:
 *****************************************************************************/
int createBracetopia(BracetopiaBoard *boardPtr, size_t size, double threshold,
                     double percVac, double percEnd) {
    boardPtr->board = allocateBoard(size);
    if (NULL != boardPtr->board) {
        boardPtr->size = size;
        boardPtr->happiness = 0.0;
        boardPtr->happThreshold = threshold;
        boardPtr->percVac = percVac;
        boardPtr->percEnd = percEnd;
        return 0;
    } else {
        fprintf(stderr, "Error allocating board. At %s:%d\n", __FILE__,
                __LINE__);
        return MALLOC_ERROR;
    }
    return 0;
}

/******************************************************************************
 * Function:         void destroyBracetopia
 * Description:      Destroys the board for bracetopia. Frees all memory used
 * Where:
 *                   BracetopiaBoard* boardPtr - pointer to the board
 * Return:           None
 * Error:
 *****************************************************************************/
void destroyBracetopia(BracetopiaBoard *boardPtr) {
    int i = 0;
    for (i = 0; i < boardPtr->size; i++) {
        free(boardPtr->board[i]);
    }
    free(boardPtr->board);
}

/******************************************************************************
 * Function:         double getCommunityHappiness
 * Description:      Calculates the average happiness fo community and updates
 *                   individual cell happiness
 * Where:
 *                   BracetopiaBoard *boardPtr - pointer to the board struct
 * Return:           double - average happiness
 * Error:            None
 *****************************************************************************/
double getCommunityHappiness(BracetopiaBoard *boardPtr) {
    double total = 0, temp = 0;
    int num = 0;
    int i, j;
    Cell *currCell;
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            currCell = &boardPtr->board[i][j];
            if (currCell->status) { // checks if not vacant
                temp = getCellHappiness(boardPtr, i, j);
                total += temp;
                num++;
            }
        }
    }
    return total / (double)num;
}

/******************************************************************************
 * Function:         double getCellHappiness
 *                   Calculate the happiness value of the cell given by (x,y)
 * Where:
 *                   BracetopiaBoard *boardPtr - Pointer to board
 *                   int x - horizontal coordinate of cell in questsion
 *                   int y - vertical coordinate of cell in question
 * Return:           float happiness: happiness value
 * Error:            None
 *****************************************************************************/
double getCellHappiness(BracetopiaBoard *boardPtr, int x, int y) {
    int i, j;
    double happiness = 0;

    uint8_t like_neighbors = 0, total_neighbors = 0;
    for (i = -1; i < 2; i++) {
        if (x + i < 0 || x + i >= boardPtr->size)
            continue;
        for (j = -1; j < 2; j++) {
            if (y + j < 0 || y + j >= boardPtr->size)
                continue;
            if (0 == i && 0 == j)
                continue;
            if (boardPtr->board[x + i][y + j].status) {
                total_neighbors++;
                if (boardPtr->board[x][y].status ==
                    boardPtr->board[x + i][y + j].status) {
                    like_neighbors++;
                }
            }
        }
    }
    if (0 != total_neighbors) {
        boardPtr->board[x][y].happiness = happiness =
            (double)like_neighbors / (double)total_neighbors;
    } else {
        boardPtr->board[x][y].happiness = happiness = 1;
    }
    return happiness;
}

/******************************************************************************
 * Function:         int move
 *                   Moves the cels between cycles
 * Where:
 *                   BracetopiaBoard *boardPtr - Pointer to board
 * Return:           int - number of agents moved
 * Error:            None
 *****************************************************************************/
int move(BracetopiaBoard *boardPtr) {
    Cell **newBoard = allocateBoard(boardPtr->size);
    Cell *currCell;
    coordinate coord = {0, 0};
    int i, j;
    int movedAgents = 0;

    /* Iterate through every cell in board */
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            /* current cell being worked with */
            currCell = &boardPtr->board[i][j];
            /* Since alignment is an enum that vacant is 0, using it in a
             * conditional is fine. This checks if not vacant */
            if (currCell->status) {
                /* check if unhappy */
                if (currCell->happiness < boardPtr->happThreshold) {
                    /* find the next open spot, that is vacant in old and new */
                    coord = findNextVacantSpace(boardPtr, newBoard);

                    /* copy it to the new coordinate */
                    /* If the coord is (-1,-1) then a spot was unable to be
                     * found, and the cell will simply be moved to its current
                     * position, even though it is unhappy*/
                    if (coord.x != -1 && coord.y != -1) {
                        movedAgents++;
                        newBoard[coord.x][coord.y] = *currCell;
                    } else {
                        newBoard[i][j] = *currCell;
                    }

                } else {
                    /* if it isn't unhappy, just move it to same spot */
                    newBoard[i][j] = *currCell;
                }
            }
        }
    }

    // destroy old board stuff
    destroyBracetopia(boardPtr); /* free the old board */
    boardPtr->board = newBoard;  /* set the newBoard to the current board */
    /* update the happiness. This also updates all of the cells' happiness
     * values */
    boardPtr->happiness = getCommunityHappiness(boardPtr);

    /* return the number of moves this cycle. */
    return movedAgents;
}

/******************************************************************************
 * Function:         Cell** allocateBoard
 * Description:      Allocates double Cell array for board. Primarily used in
 *                   move
 * Where:
 *                   int size - size of the board
 * Return:           Cell** pointer of double Cell array
 * Error:            None
 *****************************************************************************/
Cell **allocateBoard(int size) {
    int i = 0;
    Cell **board = malloc(sizeof(Cell *) * size);

    /* check if malloc failed */
    if (NULL == board) {
        fprintf(stderr, "Error while allocating board in %s:%d\n", __FILE__,
                __LINE__);
    } else {
        /* iterate through whole board and allocate each row of cells */
        for (i = 0; i < size; i++) {
            /* I used calloc, that way it Initializes everything to vacant.
             * Since occupancy is given by an enum, and 0 == vacant */
            board[i] = calloc(size, sizeof(Cell)); // makes them all vacant
            /* check if calloc failed */
            if (NULL == board[i]) {
                fprintf(stderr, "Error while allocating row: %d\n", i);
            }
        }
    }
    return board;
}

/******************************************************************************
 * Function:         coordinate findNextVacantSpace
 *                   Performs all of the logic for finding the next available
 *                   space.
 * Where:
 *                   BracetopiaBoard *boardPtr - board in qustsion
                     Cell **newBoard - the new board that is being created
 * Return:           coordinate, the next available spot
 * Error:            If no cell was eligible, then returns (-1,-1)
 *****************************************************************************/
coordinate findNextVacantSpace(BracetopiaBoard *boardPtr, Cell **newBoard) {
    coordinate coord = {0, 0};
    int i, j;
    /* iterates through entire board */
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            /* if the space is vacant on both the old and new board */
            if (!(boardPtr->board[i][j].status || newBoard[i][j].status)) {
                coord.x = i;
                coord.y = j;
                return coord;
            }
        }
    }
    /* if it has reached this point. Then no eligible spaces have been found */
    coord.x = -1;
    coord.y = -1;

    return coord;
}

/******************************************************************************
 * Function:         void populateBoard
 *                   Populate the board with random assortment of agents
 * Where:
 *                   BracetopiaBoard *boardPtr - Pointer to board
 *                   double percVac - percent of board that should
 *                      remain vacant
 *                   double percEndl - percent of agents that should be
 *                      endline agents. Newline will be
 *                      1-percentVacant-percentEndline
 * Return:           None
 * Error:            None
 *****************************************************************************/
void populateBoard(BracetopiaBoard *boardPtr, double percVac, double percEndl) {
    int numVac = (boardPtr->size * boardPtr->size) * percVac;
    int totalAgents = (boardPtr->size * boardPtr->size) - numVac;
    int endLine = totalAgents * percEndl;

    int curr = 0;
    int i, j;
    int x, y;

    Cell currCell;

    srand(41);

    /* populates the grid with unshuffled agents */
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            if (curr >= totalAgents)
                break;
            if (curr < endLine) {
                boardPtr->board[i][j].status = ENDLINE;
            } else {
                boardPtr->board[i][j].status = NEWLINE;
            }
            curr++;
        }
    }
    /* printGrid(boardPtr, 0, 0); */
    /* shuffles the board */
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            x = rand() % boardPtr->size;
            y = rand() % boardPtr->size;

            currCell = boardPtr->board[i][j];
            boardPtr->board[i][j] = boardPtr->board[x][y];
            boardPtr->board[x][y] = currCell;
        }
    }
    boardPtr->happiness = getCommunityHappiness(boardPtr);
}

/******************************************************************************
 * Function:         void printGrid
 *                   Prints the grid in boardPtr to stdout
 * Where:
 *                   BracetopiaBoard *boardPtr - Board in questsion
 * Return:           None
 * Error:            None
 *****************************************************************************/
void printGrid(BracetopiaBoard *boardPtr, int currCycle, int movesCycle) {
    int i, j;
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            /* Based on the status of the cell. Print certain char */
            switch (boardPtr->board[i][j].status) {
            case ENDLINE:
                putchar('e');
                break;
            case NEWLINE:
                putchar('n');
                break;

            default:
                putchar('.');
                break;
            }
        }
        /* Put newline after row is done */
        putchar('\n');
    }
    /* print info */
    printf("cycle: %d\n", currCycle);
    printf("moves this cycle: %d\n", movesCycle);
    printf("teams' \"happiness\": %f\n", boardPtr->happiness);
    printf("dim: %d, %%strength of preference: %d%%, %%vacancy: %d%%, %%end: "
           "%d%%\n",
           boardPtr->size, (int)(boardPtr->happThreshold * 100),
           (int)(boardPtr->percVac * 100), (int)(boardPtr->percEnd * 100));
    printf("Use Control-C to quit.\n");

    /* If compiled in debug mode, print debug output */
#ifdef DEBUG
    int *populations = calloc(3, sizeof(int));
    printf("Census Data:\n");
    census(boardPtr, populations);
    printf("Vacant: %d, Endline: %d, Newline: %d\n", populations[0],
           populations[2], populations[1]);
    free(populations);
#endif
}

/******************************************************************************
 * Function:         void census
 *                   Function to perform a census. Used for debugging
 * Where:
 *                   BracetopiaBoard *boardPtr - pointer to board
 *                   int *populations - array to store populations in
 * Return:           void
 * Error:            none
 *****************************************************************************/
void census(BracetopiaBoard *boardPtr, int *populations) {
    int i, j;
    /* iterate through board */
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            /* stores it based on the status enum value */
            populations[boardPtr->board[i][j].status]++;
        }
    }
}
