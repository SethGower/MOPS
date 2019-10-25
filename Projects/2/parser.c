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
tree_node_t *parse(stack_t *stack) {
    if (empty_stack(stack))
        return NULL;
    op_type_t op;
    char *tok = top(stack);
    pop(stack);
    char *strtolPtr;
    tree_node_t *left, *right;
    exp_type_t type;
    if (tok[0] == '+')
        op = ADD_OP;
    else if (tok[0] == '-')
        op = SUB_OP;
    else if (tok[0] == '*')
        op = MUL_OP;
    else if (tok[0] == '/')
        op = DIV_OP;
    else if (tok[0] == '%')
        op = MOD_OP;
    else if (tok[0] == '=')
        op = ASSIGN_OP;
    else if (tok[0] == '?')
        op = Q_OP;
    else if (tok[0] == ':')
        op = ALT_OP;
    else
        op = NO_OP; /* not handling ternery yet */

    if (op != NO_OP) { /* if it is an operation */
        if (op != Q_OP) {
            right = parse(stack);
            left = parse(stack);
            return make_interior(op, tok, left, right);
        } else {
            push(stack, ":");
            right = parse(stack);
            left = parse(stack);
            return make_interior(op, tok, left, right);
        }

    } else { /* if it is a value or symbol */
        strtol(tok, &strtolPtr, 10);
        /* if the two pointers are the same, then strtol failed. This means
         * that the token wasn't a number, meaning it must be a symbol */
        if (strtolPtr == tok)
            type = SYMBOL;
        else
            type = INTEGER;
        return make_leaf(type, tok);
    }
}

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
