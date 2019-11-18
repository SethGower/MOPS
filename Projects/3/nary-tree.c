#include "nary-tree.h"

void freeTree(treeNode *tree) {}
void printTree(treeNode *tree) {}
treeNode *createNode(char *name) { return NULL; }
treeNode *findNode(treeNode *tree, char *name) { return NULL; }
treeNode *addChild(treeNode *tree, char *parent, char *child) {
    treeNode *parentNode = findNode(tree, parent);
    treeNode *childNode = NULL;
    if (NULL != parentNode) {
        /*
         * There are a couple of cases going into this.
         * If numChildren == childCap == 0
         *  - This is the node's first child, and the realloc is effectively
         *    just a malloc
         * If numChildren < childCap
         *  - This gets passed over, and continues adding the child
         * If numChildren >= childCap
         *  - The children array gets resized by the GROWTH_AMOUNT and then
         *    the child is added
         * */
        if (tree->numChildren >= tree->childCap) {
            tree->children =
                realloc(tree->children, tree->childCap += GROWTH_AMOUNT);
            if (NULL == tree->children) {
                perror("addChild Realloc");
            }
        }
        childNode = &tree->children[tree->numChildren]; /* temp variable */
        childNode->name = malloc(strlen(child) + 1);    /* get space for name */
        strcpy(childNode->name, child);                 /* place name */
        childNode->numChildren = 0; /* set the node to have no children */
        childNode->childCap = 0;
        childNode->children = NULL;
        return parentNode;
    }
    return NULL;
}
