// File: bst.h
// bst.h defines the interfaces for the Binary Search Tree program
//
// @author tvf  timothy fossum
// @date Wed Sep 11 14:08:00 EDT 2019

#ifndef BST_H
#define BST_H

/// The definition of the tree structure
typedef struct TreeNode {
    int val ;                 ///< the data stored in the node
    struct TreeNode* left ;   ///< node's left child
    struct TreeNode* right ;  ///< node's right child
} TreeNode;

/// The three supported traversals
typedef enum {
    PREORDER,           ///< node (val), then left, then right
    INORDER,            ///< left, then node (val), then right
    POSTORDER           ///< left, then right, then node (val)
} TraversalType;

// FUNCTIONS STUDENTS ARE REQUIRED TO IMPLEMENT

/// build_list:
///     Dynamically builds a linked list of TreeNodes, using
///     the 'left' fields as links, by repeatedly reading an integer
///     value from standard input, allocating a TreeNode structure
///     with the value in its 'val' field, and linking this node to
///     the previous node's 'left' field. The first TreeNode constructed
///     becomes the head of the list, and a pointer to it is returned
///     by the function. If there are no integer values in the input, 
///     NULL is returned.
/// @return  ptr to the head of the TreeNode list, or NULL for an empty list
TreeNode* build_list(void);

/// reconstruct:
///     Reconstructs the BST from the TreeNode pointer returned
///     from build_list. This function only modifies the 'left' and
///     'right' fields of the TreeNodes in the list. The original 
///     TreeNode becomes the root of the BST.
/// @param head  pointer to the head of the TreeNode list, or NULL
/// @return  true if BST reconstruction is OK, false on failure
bool reconstruct(TreeNode* head);

/// traverse()
///    Traverses the tree in preorder, inorder, or
///    postorder form (depending on the given traversal type),
///    displaying the value of each node visited to standard output.
/// @param root  pointer to the root of the BST
/// @param type  the desired traversal (a TraversalType enum)
void traverse(const TreeNode* root, const TraversalType type);

/// cleanup()
///    Cleanup all memory allocated to the TreeNodes
/// @param root  pointer to the root of the tree to be deallocated
void cleanup(TreeNode* root);


#endif // BST_H
