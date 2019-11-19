#include "nary-tree.h"
#include "queueADT.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

#define INITIAL_SIZE 10
#define GROWTH_AMOUNT 10
void freeTree(treeNode *tree) {
    size_t i = 0;
    if (NULL != tree) {
        free(tree->name);
        for (i = 0; i < tree->numChildren; i++) {
            freeTree(&tree->children[i]);
        }
        free(tree->children);
    }
}
void printTree(treeNode *tree) {
    if (NULL != tree) {
        queue_t q = que_create();
        treeNode *currNode = NULL;
        size_t i = 0;
        que_enqueue(q, (void *)tree);
        char *buff = (char *)malloc(1024);
        buff[0] = 0;
        while (que_size(q) > 0) {
            currNode = (treeNode *)que_front(q);
            que_dequeue(q);
            strcat(buff, currNode->name);
            strcat(buff, " had ");
            if (currNode->numChildren) {
                strcat(buff, currNode->children[0].name);
                for (i = 1; i < currNode->numChildren - 1; i++) {
                    strcat(buff, ", ");
                    strcat(buff, currNode->children[i].name);
                }
                if (currNode->numChildren > 1) {
                    strcat(buff, " and ");
                    strcat(buff,
                           currNode->children[currNode->numChildren - 1].name);
                }
                strcat(buff, ".");
            } else {
                strcat(buff, "no offspring.");
            }
            printf("%s\n", buff);
            buff[0] = 0;
        }
        free(buff);
        que_destroy(q);
    }
}
treeNode *createTree(const char *name) {
    treeNode *tree = calloc(1, sizeof(treeNode));
    if (NULL == tree) {
        perror("Create Tree");
    } else {
        tree->name = malloc(strlen(name) + 1);
        tree->numChildren = 0;
        tree->childCap = 0;
        strcpy(tree->name, name);
    }
    return tree;
}
treeNode *findNode(treeNode *tree, char *name) {
    queue_t q = que_create();
    treeNode *currNode = NULL;
    size_t i = 0;
    que_enqueue(q, (void *)tree);
    while (que_size(q) > 0) {
        currNode = (treeNode *)que_front(q);
        que_dequeue(q);
        if (!strcmp(currNode->name, name)) {
            que_destroy(q);
            return currNode;
        } else {
            for (i = 0; i < currNode->numChildren; i++) {
                que_enqueue(q, (void *)&currNode->children[i]);
            }
        }
    }
    que_destroy(q);
    return NULL;
}
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
        if (parentNode->numChildren >= parentNode->childCap) {
            parentNode->children = realloc(
                parentNode->children,
                (parentNode->childCap += GROWTH_AMOUNT) * sizeof(treeNode));
            if (NULL == parentNode->children) {
                perror("addChild Realloc");
            }
        }
        childNode =
            &parentNode->children[parentNode->numChildren]; /* temp variable */
        childNode->name = malloc(strlen(child) + 1); /* get space for name */
        strcpy(childNode->name, child);              /* place name */
        childNode->numChildren = 0; /* set the node to have no children */
        childNode->childCap = 0;
        childNode->children = NULL;
        parentNode->numChildren++;
        return parentNode;
    }
    return NULL;
}
size_t treeSize(treeNode *tree) {
    size_t i = 0;
    size_t num = 0;
    if (NULL != tree) {
        num++;
        for (i = 0; i < tree->numChildren; i++) {
            num += treeSize(&tree->children[i]);
        }
    }
    return num;
}
