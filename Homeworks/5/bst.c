/******************************************************************************
 * File:             bst.c
 *
 * Author:           Seth Gower
 * Created:          10/10/19
 *                   Source code for CS243 Homework 5 - Binary Search Tree
 *****************************************************************************/
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    TreeNode *bst = build_list();
    traverse(bst, INORDER);
    cleanup(bst);
    return 0;
}

/******************************************************************************
 * Function:         TreeNode *build_list
 * Description:      Creates a linked list binary search tree from given values
 *                   that were read in from preorder
 *
 * Return:           Pointer to binary search tree
 * Error:            Returns NULL if unable to
 *****************************************************************************/
TreeNode *build_list(void) {
    TreeNode *head = NULL, *last = NULL, *curr;
    int value;
    while (fscanf(stdin, "%d", &value) != EOF) {
        curr = malloc(sizeof(TreeNode));
        curr->val = value;
        curr->left = NULL;
        curr->right = NULL;
        if (NULL == head)
            head = curr;
        if (NULL != last) {
            if (curr->val < last->val)
                last->left = curr;
            else
                last->right = curr;
        }
        last = curr;
    }

    return head;
}

/******************************************************************************
 * Function:         bool reconstruct
 *                   Reconstructs BST from the TreeNode pointer passed in.
 * Where:
 *                   TreeNode *head - TODO
 * Return:           Whether or not it failed
 * Error:            False if unable to reconstruct
 *****************************************************************************/
bool reconstruct(TreeNode *head) { return true; }

/******************************************************************************
 * Function:         void traverse
 * Description:      Traverses given tree, without modifying. Traverses based
 *                   on the given TraversalType
 * Where:
 *                   const TreeNode *root - Root Node for function
 *                   const TraversalType type - Type of traversal to perform
 * Return:           None
 * Error:            None
 *****************************************************************************/
void traverse(const TreeNode *root, const TraversalType type) {
    if (NULL != root) {
        switch (type) {
        case PREORDER:
            printf("%d\n", root->val);
            ;
            traverse(root->left, type);
            traverse(root->right, type);
            break;
        case INORDER:
            traverse(root->left, type);
            printf("%d\n", root->val);
            traverse(root->right, type);
            break;
        case POSTORDER:
            traverse(root->left, type);
            traverse(root->right, type);
            printf("%d\n", root->val);
        }
    }
}

/******************************************************************************
 * Function:         void cleanup
 *                   Recursively goes through tree and frees each node.
 * Where:
 *                   TreeNode *root - TODO
 * Return:           None
 * Error:            None
 *****************************************************************************/
void cleanup(TreeNode *root) {
    if (NULL != root) {
        if (root->left)
            cleanup(root->left);
        if (root->right)
            cleanup(root->right);
    }
    free(root);
}
