/******************************************************************************
 * File:             interp.c
 *
 * Author:           Seth Gower
 * Created:          10/18/19
 *                   Main program file for CSCI-243 Project 2
 *****************************************************************************/

#include "interp.h"
#include "parser.h"
#include "stack.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *buff = malloc(BUFLEN);
    build_table("sym.1");
    rep("a b + c * 7 +");
    rep("0 3 5 ?");
    free(buff);
    free_table();
    return 0;
}
