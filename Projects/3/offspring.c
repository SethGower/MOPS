#define _DEFAULT_SOURCE
#include "nary-tree.h"
#include "trimit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_SIZE 1024
int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    char *buff = malloc(MAX_BUF_SIZE);
    char *parentName;
    char *childName;
    size_t buffSize = MAX_BUF_SIZE;
    treeNode *tree = NULL;
    /* check if a filename was passed in */
    if (argc >= 2) {
        fp = fopen(argv[1], "r");             /* open the file */
        if (NULL == fp) {                     /* check if it failed */
            perror("Offspring input file: "); /*print error on failure*/
            return EXIT_FAILURE;
        } else {
            /* loop through entire file, saving lines to buff */
            while (getline(&buff, &buffSize, fp) > 0) {
                /* if there is a newline character, delete it */
                if (buff[strlen(buff) - 1] == '\n')
                    buff[strlen(buff) - 1] = 0;
                buff = trim(buff);              /* trim the buffer */
                parentName = strtok(buff, ","); /* tokenize the string */
                parentName = trim(parentName);
                if (!tree) /* check if this is the root node */
                    tree = createTree(parentName); /* if so, create tree */
                /* loop through all children in the line */
                while ((childName = strtok(NULL, ","))) {
                    childName = trim(childName);
                    addChild(tree, parentName, childName);
                }
            }
            fclose(fp);
            printTree(tree);
            printf("%ld\n", treeSize(tree));
            freeTree(tree);
            free(tree);
        }
    }
    free(buff);
    return 0;
}
