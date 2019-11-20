#ifndef NARY_TREE_H
#define NARY_TREE_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct treeNode {
    char *name;
    size_t numChildren;
    size_t childCap;
    struct treeNode *children;
} treeNode;

/******************************************************************************
 * Function:         void freeTree
 *                   Recursively frees the tree given by the tree argument
 * Where:
 *                   treeNode *tree - tree that is being freed
 * Return:           None
 * Error:            None
 *****************************************************************************/
void freeTree(treeNode *tree);

/******************************************************************************
 * Function:         void printTree
 *                   Prints the contents of the tree to stdout
 * Where:
 *                   treeNode *tree - tree that is being printed
 * Return:           None
 * Error:            None
 *****************************************************************************/
void printTree(treeNode *tree);

/******************************************************************************
 * Function:         treeNode *createNode
 *                   Creates a new node in the tree
 * Where:
 *                   char *name - root name for the tree
 * Return:           The pointer to the node that was created
 * Error:            returns NULL if an error occured and sets errno
 *****************************************************************************/
treeNode *createTree(const char *name);

/******************************************************************************
 * Function:         treeNode *findNode
 * Description:      Returns the pointer to the node with the name given as an
 *                   argument
 * Where:
 *                   treeNode *tree - tree that is being searched for a
 *                                    specific name
 *                   char *name - name to be searched for
 * Return:           the pointer to the node
 * Error:            NULL if there is no node with that name
 *****************************************************************************/
treeNode *findNode(treeNode *tree, char *name);

/******************************************************************************
 * Function:         treeNode *addChild
 * Description:      Creates a child in the tree, by finding the parent given
 *                   by "parent" and creates a child with the name "child"
 * Where:
 *                   treeNode *tree - tree that is getting the child
 *                   char *parent - name of the parent to give the child
 *                   char *child - name of the child
 * Return:           The pointer to the parent node
 * Error:            returns NULL if there was no parent with that name.
 *****************************************************************************/
treeNode *addChild(treeNode *tree, char *parent, char *child);

/******************************************************************************
 * Function:         size_t treeSize
 *                   Return the number of nodes in the tree
 * Where:
 *                   treeNode *tree - tree to check the size of
 * Return:           size_t - number of nodes in tree
 * Error:            None
 *****************************************************************************/
size_t treeSize(treeNode *tree);

/******************************************************************************
 * Function:         void printNode
 *                   Prints the Name and name of all children of a single node
 * Where:
 *                   treeNode* node - tree node to print. Only prints info
 *                                    about current node, doesn't do BFS
 * Return:           None
 * Error:            None
 *****************************************************************************/
void printNode(treeNode *node);

/******************************************************************************
 * Function:         size_t treeHeight
 *                   Gets the height of the tree given in parameters
 * Where:
 *                   treeNode *tree - tree of which the height is being
 *                                    checked
 * Return:           size_t - the height of the tree
 * Error:            None
 *****************************************************************************/
size_t treeHeight(treeNode *tree);
#endif
