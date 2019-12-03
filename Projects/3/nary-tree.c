#include "nary-tree.h"
#include "queue.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

#define INITIAL_SIZE 10
#define GROWTH_AMOUNT 10
/* compare is solely used for the qsort call in treeHeight */
int compare(const void *a, const void *b) { return *(int *)a - *(int *)b; }
void freeTree(treeNode *tree) {
    size_t i = 0;
    if (NULL != tree) {
        free(tree->name);
        for (i = 0; i < tree->numChildren; i++) {
            freeTree(&tree->children[i]); /* recursively free all children */
        }
        free(tree->children);
    }
}
void printTree(treeNode *tree) {
    if (NULL != tree) {
        size_t i = 0;
        queue_t q = que_create();
        treeNode *currNode = NULL;
        que_enqueue(q, (void *)tree);
        /* performs a breadth first search starting from given node */
        while (que_size(q) > 0) {
            currNode = (treeNode *)que_dequeue(q); /* gets parent off queue */
            printf("%s had ", currNode->name);
            if (currNode->numChildren) { /* if they have children, print them */
                for (i = 0; i < currNode->numChildren; i++)
                    /* enqueue all of the children  */
                    que_enqueue(q, (void *)&currNode->children[i]);
                /* print the first child, may be only child */
                printf("%s", currNode->children[0].name);
                /* print children [1,numChildren) preceded by comma */
                for (i = 1; i < currNode->numChildren - 1; i++) {
                    printf(", %s", currNode->children[i].name);
                }
                /* if there are more than 1 children, then print last child
                 * preceded by 'and' without a comma */
                if (currNode->numChildren > 1) {
                    printf(" and %s",
                           currNode->children[currNode->numChildren - 1].name);
                }
                /* end this lind of output with period and newline */
                printf(".\n");

            } else { /* if they didn't have kids, print that */
                printf("no offspring.\n");
            }
        }
        que_destroy(q); /* free the queue */
    }
}
treeNode *createTree(const char *name) {
    /* allocate space for tree structure, and initialize to zero to prevent mem
     * errors reading */
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
    if (NULL != tree) {
        queue_t q = que_create();
        treeNode *currNode = NULL;
        size_t i = 0;
        /* perform the breadth first search for the name */
        que_enqueue(q, (void *)tree);
        while (que_size(q) > 0) {
            currNode = (treeNode *)que_dequeue(q);
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
    }
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
        /* temp variable to reduce '->' and irritation */
        childNode = &parentNode->children[parentNode->numChildren++];
        childNode->name = malloc(strlen(child) + 1); /* get space for name */
        strcpy(childNode->name, child);              /* place name */
        childNode->numChildren = 0; /* set the node to have no children */
        childNode->childCap = 0;
        childNode->children = NULL;
        return parentNode;
    } else {
        /* if the parent was not found, then print the error message */
        fprintf(stderr,
                "error: '%s' is not in the tree and '%s' is not the root\n",
                parent, child);
    }
    return NULL;
}
size_t treeSize(treeNode *tree) {
    size_t i = 0;
    size_t num = 0;
    if (NULL != tree) {
        num++;
        for (i = 0; i < tree->numChildren; i++) {
            /* recursively check size of each child, adding to total */
            num += treeSize(&tree->children[i]);
        }
    }
    return num;
}
void printNode(treeNode *node) {
    if (NULL != node) {
        printf("%s had ", node->name);
        if (node->numChildren) { /* if they have children, print them */
            /* print the first child, may be only child */
            printf("%s", node->children[0].name);
            /* print children [1,numChildren) preceded by comma */
            /* if there are more than 1 children, then print last child
             * preceded by 'and' without a comma */
            if (node->numChildren > 1) {
                printf(" and %s", node->children[node->numChildren - 1].name);
            }
            /* end this lind of output with period and newline */
            printf(".\n");

        } else { /* if they didn't have kids, print that */
            printf("no offspring.\n");
        }
    }
}
int treeHeight(treeNode *tree) {
    int height = 0; /* running height */
    size_t i = 0;   /* loop variable */
    int *list;      /* list for the heights of children */
    if (NULL == tree)
        height = 0; /* base case for recursion */
    else {
        if (tree->numChildren) {
            height = 1; /* if it has at least one child, it is guaranteed to
                           have a height of at least 1 */
            list = (int *)calloc(tree->numChildren, sizeof(int));
            for (i = 0; i < tree->numChildren; i++) /* add children heights */
                list[i] = treeHeight(&tree->children[i]);
            /* sort the list of heights */
            qsort((void *)list, tree->numChildren, sizeof(int), compare);
            /* the highest height is the height of this node, so add it to the
             * running height (which is 1) */
            height += list[tree->numChildren - 1];
            free(list);
        } else {
            /* if no children, then height is 0 */
            height = 0;
        }
    }
    return (size_t)height;
}
