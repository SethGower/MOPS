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

    build_table("sym.1");
    rep("0 b c ?");

    return 0;
}
