/******************************************************************************
 * File:             bracetopia.h
 *
 * Author:           Seth Gower
 * Created:          10/07/19
 *                   Header file to provide definitions for bracetopia
 *****************************************************************************/

#ifndef _BRACETOPIA_H
#define _BRACETOPIA_H
#define _DEFAULT_SOURCE
#include <stdlib.h>

typedef enum { VACANT = 0, NEWLINE = 1, ENDLINE = 2 } alignment;

typedef struct {
    double happiness;
    alignment status;
} Cell;

typedef struct {
    int size;
    double happiness;
    Cell **board;
    double happThreshold;
    double percVac;
    double percEnd;
} BracetopiaBoard;

typedef struct {
    int x, y;
} coordinate;

int createBracetopia(BracetopiaBoard *boardPtr, size_t size, double threshold,
                     double percVac, double percEnd);
void destroyBracetopia(BracetopiaBoard *boardPtr);
double getCommunityHappiness(BracetopiaBoard *boardPtr);
double getCellHappiness(BracetopiaBoard *boardPtr, int x, int y);
int move(BracetopiaBoard *boardPtr);
Cell **allocateBoard(int size);
coordinate findNextVacantSpace(BracetopiaBoard *boardPtr, Cell **newBoard);
void populateBoard(BracetopiaBoard *boardPtr, double percVac, double percEndl);
void printGrid(BracetopiaBoard *boardPtr, int currCycle, int movesCycle);
void census(BracetopiaBoard *boardPtr, int *populations);
#endif
