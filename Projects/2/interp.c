/******************************************************************************
 * File:             interp.c
 *
 * Author:           Seth Gower
 * Created:          10/18/19
 *                   Main program file for CSCI-243 Project 2
 *****************************************************************************/

#include "interp.h"
#include "stack.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("%d\n", argc);

    build_table(argv[1]);
    dump_table();

    printf("Symbol: %s has Value: %d\n", "x", lookup_table("x")->val);
    free_table();
    return 0;
}
