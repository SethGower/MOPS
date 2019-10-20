/******************************************************************************
 * File:             interp.c
 *
 * Author:           Seth Gower
 * Created:          10/18/19
 *                   Main program file for CSCI-243 Project 2
 *****************************************************************************/

#include "interp.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    stack_t *stack = make_stack();
    int i = 0;
    int *xp;
    for (i = 0; i < 20; i++) {
        xp = malloc(sizeof(int));
        *xp = i;
        push(stack, xp);
    }
    for (i = 0; i < 20; i++) {
        printf("%d\n", *(int *)top(stack));
        pop(stack);
    }
    free_stack(stack);
    return 0;
}
