/******************************************************************************
 * File:             bracetopia.h
 *
 * Author:           Seth Gower
 * Created:          10/07/19
 *                   Header file to provide definitions for bracetopia
 *****************************************************************************/

#ifndef _BRACETOPIA_H
#define _BRACETOPIA_H
#include <stdlib.h>

#define _DEFAULT_SOURCE

typedef enum { VACANT, NEWLINE, ENDLINE } alignment;

typedef struct {
    double happiness;
    alignment status;
} Cell;

typedef struct {
    int size;
    double happiness;
    Cell **board;
    double happThreshold;
} BracetopiaBoard;

typedef struct {
    int x, y;
} coordinate;

int createBracetopia(BracetopiaBoard *boardPtr, size_t size,
                     double happThreshold);
void destroyBracetopia(BracetopiaBoard *boardPtr);
double getCommunityHappiness(BracetopiaBoard *boardPtr);
double getCellHappiness(BracetopiaBoard *boardPtr, int x, int y);
int move(BracetopiaBoard *boardPtr);
Cell **allocateBoard(int size);
coordinate findNextVacantSpace(BracetopiaBoard *boardPtr, int x, int y);
#endif
