#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
/******************************************************************************
 * Function:         stack_t *make_stack
 *                   creates an empty stack.
 * Where:
 * Return:           stack_t structure with a top pointer of NULL
 * Error:
 *****************************************************************************/
stack_t *make_stack() {
    /* I don't like the fact that the struct itself is stored on the heap, I
     * personally think it is better if record structurs for data structures,
     * like the stack should be stored on the actual program stack, and if
     * need be, the address for it are passed into functions. If instead of
     * the way this function works, it just took in an address to a stack to
     * "create", the user can decide whether or not to store it on the heap or
     * stack. */
    stack_t *stack = (stack_t *)calloc(1, sizeof(stack_t));
    stack->top = NULL;
    return stack;
}

/******************************************************************************
 * Function:         void push
 *                   adds an element to the top of the stack
 * Where:
 *                   stack_t *stack - Top of the stack
 *                   void *data - the data to add to the stack
 *****************************************************************************/
void push(stack_t *stack, void *data) {
    /* Just a note. I don't think that the way the stack_node_t type is
     * defined is a great way to do this. I think it would be better if the
     * data was stored in the node and not a pointer to it. Since this is more
     * spread out than it needs to be. */
    stack_node_t *node = (stack_node_t *)calloc(1, sizeof(stack_node_t));
    node->data = data;
    node->next = stack->top;
    stack->top = node;
}

/******************************************************************************
 * Function:         void *top
 *                   returns the element on the top of the stack
 * Where:
 *                   stack_t *stack - stack that is being checked
 * Return:           a pointer to the element on the top of th estack
 * Error:            if stack is empty, exit with EXIT_FAILURE
 *****************************************************************************/
void *top(stack_t *stack) {
    if (!empty_stack(stack)) {
        return stack->top->data;
    } else {
        fprintf(
            stderr,
            "Error when checking top of stack. Stack is empty. Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/******************************************************************************
 * Function:         void pop
 * Description:      removes the element at the top of the stack. But doesn't
 *                   return it for some reason, since this was provided
 *                   prototype
 * Where:
 *                   stack_t *stack - stack being modified
 * Return:
 * Error:            if stack is empty, exit with EXIT_FAILURE
 *****************************************************************************/

void pop(stack_t *stack) {
    /* Just a node. I personally don't think that this is a great way to do this
     * function, but I wasn't allowed to change the provided prototypes. I think
     * that it would be better if the pop function also did the functionality of
     * the top() function. That way you don't need to call two functions */
    if (!empty_stack(stack)) {
        stack_node_t *node = stack->top;
        stack->top = stack->top->next;
        free(node->data);
        free(node);
    } else {
        fprintf(stderr, "Error while popping. Empty stack. Exiting...\n");
        exit(EXIT_FAILURE);
    }
}

/******************************************************************************
 * Function:         int empty_stack
 *                   checks if the stack is empty
 * Where:
 *                   stack_t *stack - pointer to the top of stack
 * Return:           1 if stack is empty. 0 if not empty
 *****************************************************************************/
int empty_stack(stack_t *stack) {
    /* returns if the top node is NULL */
    return NULL == stack->top;
}

/******************************************************************************
 * Function:         void free_stack
 *                   frees all memory associated with the stack
 * Where:
 *                   stack_t *stack - pointer to the stack
 *****************************************************************************/
void free_stack(stack_t *stack) {
    /* sets up the nodes for traversal. I decided to free iteratively instead
     * of recursively */
    stack_node_t *currNode = stack->top;
    stack_node_t *nextNode;
    while (currNode) {
        nextNode = currNode->next;
        /* frees the data and then the current node. */
        free(currNode->data);
        free(currNode);
        currNode = nextNode;
    }
    /* finally frees the actual stack structure */
    free(stack);
}
