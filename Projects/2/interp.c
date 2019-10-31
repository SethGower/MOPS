/******************************************************************************
 * File:             interp.c
 *
 * Author:           Seth Gower
 * Created:          10/18/19
 *                   Main program file for CSCI-243 Project 2
 *****************************************************************************/

#define _DEFAULT_SOURCE
#include "interp.h"
#include "parser.h"
#include "stack.h"
#include "symtab.h"
#include "trimit.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *buff = malloc(BUFLEN); /* buffer to store the line */
    size_t buffLen = BUFLEN;     /* buffer size to pass into getline */
    int errorCode;               /* temp variable to store errno */
    if (argc == 2) {             /* check if symbol table was passd in */
        build_table(argv[1]);    /* build_table handles file checking */
        dump_table();            /* print table */
    } else if (argc > 2) {       /* if too many arguments */
        fprintf(stderr, "Usage: interp [sym-table]\n");
        return (EXIT_FAILURE);
    }
    /* enter the REP loop.  */
    printf("Enter postfix expressions (CTRL-D to exit):\n");
    printf("> ");
    while (getline(&buff, &buffLen, stdin) >= 0) {
        if ((errorCode = errno)) { /* check if getline failed (unable to
                                      allocate memory) */
            perror("interp getline()");
            return (errorCode);
        }
        buff = trim(buff);  /* simply trim leading and trailing whitespace */
        if (buff[0] == '#') /* check if comment */
            continue;
        rep(buff);    /* actually parse and evaluate expression */
        printf("> "); /* start new expression */
    }
    dump_table(); /* print table */

    free(buff);   /* free line buffer */
    free_table(); /* free up table memory */
    return 0;
}
