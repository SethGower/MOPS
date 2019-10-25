#include "parser.h"
#include "stack.h"
#include "symtab.h"
#include "tree_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * Function:         void rep
 *                   Function for reading, evaluating and parsing expression
 * Where:
 *                   char *exp - Expression to evaluate
 * Return:           None
 * Error:            Prints to stderr and exits if invalid expression
 *****************************************************************************/
void rep(char *exp) {
    char *token;
    stack_t *stack = make_stack();
    tree_node_t *tree;

    char buff[BUFLEN];
    strcpy(buff, exp);

    /* read in the expression and generate stack */
    token = strtok(buff, " ");
    do {
        if (token[0] != '#') {
            push(stack, token);
        } else {
            break;
        }
    } while ((token = strtok(NULL, " ")));

    tree = parse(stack);
    printf("%d\n", eval_tree(tree));
}

/******************************************************************************
 * Function:         tree_node_t *parse
 *                   Recursively build the parse tree from the stack
 * Where:
 *                   stack_t *stack - TODO
 * Return:           Interpretation tree resulting from stack
 * Error:
 *****************************************************************************/
tree_node_t *parse(stack_t *stack) { return NULL; }

/******************************************************************************
 * Function:         tree_node_t *make_parse_tree
 *                   creates the parse tree from the expression
 * Where:
 *                   char *expr - TODO
 * Return:           The parse tree
 * Error:
 *****************************************************************************/
tree_node_t *make_parse_tree(char *expr) { return NULL; }

/******************************************************************************
 * Function:         int eval_tree
 *                   Evaluates the contents of the parse tree
 * Where:
 *                   tree_node_t *node - TODO
 * Return:           Integer result of the expression in the parse tree
 *****************************************************************************/
int eval_tree(tree_node_t *node) { return 0; }

/******************************************************************************
 * Function:         void print_infix
 *                   Prints the infix odering of the expression, given the tree
 * Where:
 *                   tree_node_t *node - TODO
 *****************************************************************************/
void print_infix(tree_node_t *node) {}
