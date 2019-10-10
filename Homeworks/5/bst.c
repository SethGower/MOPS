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

bool validBST(TreeNode *start, int rootVal);

int main() {
    TreeNode *bst = build_list();
    bool succ = reconstruct(bst);
    if (succ) {
        if (NULL != bst) {
            printf("Preoder Traversal:\n");
            traverse(bst, PREORDER);
            printf("Inorder Traversal:\n");
            traverse(bst, INORDER);
            printf("Postorder Traversal:\n");
            traverse(bst, POSTORDER);
        } else {
            printf("Empty BST\n");
        }
    } else {
        fprintf(stderr, "Input is not a BST preorder traversal\n");
        return 1;
    }
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
            last->left = curr;
        }
        last = curr;
    }

    return head;
}

/******************************************************************************
 * Function:         bool reconstruct
 *                   Reconstructs BST from the TreeNode pointer passed in.
 * Where:
 *                   TreeNode *head - the current node being reconstructed
 * Return:           Whether or not it failed
 * Error:            False if unable to reconstruct
 *****************************************************************************/
bool reconstruct(TreeNode *head) {
    TreeNode *curr, *prev;
    if (NULL == head)
        return true;
    curr = head;
    while (curr) {
        if (curr->val > head->val) {
            head->right = curr;
            prev->left = NULL;
            break;
        }
        prev = curr;
        curr = curr->left;
    }
    reconstruct(head->left);
    reconstruct(head->right);
    return validBST(head->right, head->val);
}

/******************************************************************************
 * Function:         bool validBST
 *                   Recursive function to test if BST is valid
 * Where:
 *                   TreeNode *start - Node to start checking
 *                   int rootVal - Overall root value. To check against
 *
 * Return:           boolean - true if valid, false if not
 * Error:            false if not valid
 *****************************************************************************/
bool validBST(TreeNode *start, int rootVal) {
    bool leftGood = false, rightGood = false;
    if (NULL == start) { /* a NULL node can't be invalid */
        return true;
    }
    /* Check if current value is greater than overall root */
    if (start->val > rootVal) {
        leftGood = validBST(start->left, rootVal);
        rightGood = validBST(start->right, rootVal);
        /* if not greater, then this value is not good */
    } else {
        /* print error output */
        fprintf(stderr, "Error at value %d\n", start->val);
        return false;
    }
    /* if both branches are valid, then the current is valid */
    return leftGood && rightGood;
}

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
        /* print according to the TraversalType */
        switch (type) {
        case PREORDER:
            printf("%d\n", root->val);
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
 *                   TreeNode *root - node to cleanup
 * Return:           None
 * Error:            None
 *****************************************************************************/
void cleanup(TreeNode *root) {
    if (NULL != root) {
        /* check if the nodes are not NULL */
        if (root->left)
            cleanup(root->left);
        if (root->right)
            cleanup(root->right);
    }
    /* free current node and set to NULL to remove dangling pointer */
    free(root);
    root = NULL;
}
