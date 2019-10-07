/******************************************************************************
 * File:             bracetopia.c
 *
 * Author:           Seth Gower
 * Created:          10/07/19
 * Description:      Module for Bracetopia for Project 1 for CSCI-243
 *****************************************************************************/

#include "bracetopia.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * Function:         createBracetopia
 * Description:      Initializes the struct for the board
 * Where:
 *                   BracetopiaBoard *boardPtr - pointer to board
 *                   size_t size - square size of the board
 *                   double threshold - happiness threshold for program
 *                    -
 * Return:
 * Error:
 *****************************************************************************/
int createBracetopia(BracetopiaBoard *boardPtr, size_t size, double threshold) {
    boardPtr->size = size;
    boardPtr->happiness = 0.0;
    boardPtr->happThreshold = threshold;
    boardPtr->board = allocateBoard(size);
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

double getCommunityHappiness(BracetopiaBoard *boardPtr) {
    double total;
    int num;
    int i, j;
    Cell curr_cell;
    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            curr_cell = boardPtr->board[i][j];
            if (!curr_cell.status) { // checks if not vacant
                total += getCellHappiness(boardPtr, i, j);
                num++;
            }
        }
    }
    return total / num;
}

/******************************************************************************
 * Function:         double getCellHappiness
 *                   Calculate the happiness value of the cell given by (x,y)
 * Where:
 *                   BracetopiaBoard *boardPtr - Pointer to board
 *                   size_t x - horizontal coordinate of cell in questsion
 *                   size_t y - vertical coordinate of cell in question
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
            if (!boardPtr->board[x + i][y + j].status) {
                total_neighbors++;
                if (boardPtr->board[x][y].status ==
                    boardPtr->board[x + i][y + j].status) {
                    like_neighbors++;
                }
            }
        }
    }
    boardPtr->board[x][y].happiness = happiness =
        like_neighbors / total_neighbors;
    return happiness;
}

/******************************************************************************
 * Function:         void move
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

    for (i = 0; i < boardPtr->size; i++) {
        for (j = 0; j < boardPtr->size; j++) {
            currCell = &boardPtr->board[i][j];
            if (currCell->happiness < boardPtr->happThreshold) {
                movedAgents++;
                do {
                    coord = findNextVacantSpace(boardPtr, i, j);
                } while (newBoard[coord.x][coord.y].status);
                memcpy(&newBoard[coord.x][coord.y], currCell, sizeof(Cell));

            } else {
                memcpy(&newBoard[i][j], currCell, sizeof(Cell));
            }
        }
    }

    free(boardPtr->board);
    boardPtr->board = newBoard;
    boardPtr->happiness = getCommunityHappiness(boardPtr);

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

    if (NULL == board) {
        fprintf(stderr, "Error while allocating board in %s:%d\n", __FILE__,
                __LINE__);
        return board; // prevent access of NULL pointer
    }
    for (i = 0; i < size; i++) {
        board[i] = calloc(size, sizeof(Cell)); // makes them all vacant
    }
    return board;
}

/******************************************************************************
 * Function:         coordinate findNextVacantSpace
 *                   Returns the next vacant spot in the grid
 * Where:
 *                   BracetopiaBoard *boardPtr - board in qustsion
 *                   int x - starting x coordinate
 *                   int y - starting y coordinate
 * Return:           coordinate, the next available spot
 * Error:            None
 *****************************************************************************/
coordinate findNextVacantSpace(BracetopiaBoard *boardPtr, int x, int y) {
    coordinate coord = {x, y};

    for (; coord.x < boardPtr->size; coord.x++) {
        for (; coord.y < boardPtr->size; coord.y++) {
            if (boardPtr->board[coord.x][coord.y].status) // checks if not
                                                          // vacant
                continue;
            else
                return coord;
        }
    }
    return coord;
}
