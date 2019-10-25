/******************************************************************************
 * File:             tree_node.c
 *
 * Author:           Seth Gower
 * Created:          10/25/19
 *                   Module file for treenode for project 2 for CSCI-243
 *****************************************************************************/

#include "tree_node.h"
#include <stdlib.h>

tree_node_t *make_interior(op_type_t op, char *token, tree_node_t *left,
                           tree_node_t *right) {
    tree_node_t *node = (tree_node_t *)calloc(1, sizeof(tree_node_t));
    interior_node_t *inode =
        (interior_node_t *)calloc(1, sizeof(interior_node_t));
    node->type = INTERIOR;
    node->node = (void *)inode;
    node->token = token;
    inode->left = left;
    inode->right = right;
    inode->op = op;
    return node;
}

tree_node_t *make_leaf(exp_type_t exp_type, char *token) {
    tree_node_t *node = (tree_node_t *)malloc(sizeof(tree_node_t));
    leaf_node_t *lnode = (leaf_node_t *)malloc(sizeof(leaf_node_t));
    lnode->exp_type = exp_type;
    node->node = (void *)lnode;
    node->token = token;
    node->type = LEAF;
    return node;
}
