#define _DEFAULT_SOURCE
#include "nary-tree.h"
#include "trimit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_SIZE 1024
void processLine(treeNode **tree, char *buff);
int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    char *buff = malloc(MAX_BUF_SIZE);
    size_t buffSize = MAX_BUF_SIZE;
    treeNode *tree = NULL;
    const char *prompt = "offspring> ";
    const char *helpText = "\
User Commands for offspring:\n\
add parent-name, child-name # find parent and add child.\n\
find name     # search and print name and children if name is found.\n\
print [name]  # breadth first traversal of offspring from name.\n\
size [name]   # count members in the [sub]tree.\n\
height [name] # return the height of [sub]tree.\n\
init          # delete current tree and restart with an empty tree.\n\
help          # print this information.\n\
quit          # delete current tree and end program.";
    char *command;
    treeNode *currNode;
    /* check if a filename was passed in */
    if (argc >= 2) {
        fp = fopen(argv[1], "r");             /* open the file */
        if (NULL == fp) {                     /* check if it failed */
            perror("Offspring input file: "); /*print error on failure*/
            return EXIT_FAILURE;
        } else {
            /* loop through entire file, saving lines to buff */
            while (getline(&buff, &buffSize, fp) > 0) {
                processLine(&tree, buff);
            }
            fclose(fp);
        }
    }
    while (true) {
        printf("%s", prompt);
        if (getline(&buff, &buffSize, stdin) > 0) {
            if (buff[strlen(buff) - 1] == '\n')
                buff[strlen(buff) - 1] = 0;
            buff = trim(buff); /* trim the buffer */
            printf("+ %s\n", buff);
            command = strtok(buff, " ");
            command = trim(command);
            if (!strcmp(command, "add")) {
                command = strtok(NULL, "");
                if (NULL == command) {
                    fprintf(stderr, "You must specify a name or list of names "
                                    "to add to the tree\n");
                    continue;
                }
                processLine(&tree, command);

            } else if (!strcmp(command, "find")) {
                command = strtok(NULL, "");
                if (NULL == command)
                    command = "";
                currNode = findNode(tree, command);
                if (NULL == currNode) {
                    fprintf(stderr, "error: '%s' not found.\n", command);
                    continue;
                }
                printNode(currNode);

            } else if (!strcmp(command, "print")) {
                command = strtok(NULL, "");
                if (NULL == command) {
                    printTree(tree);
                } else {
                    currNode = findNode(tree, command);
                    printTree(currNode);
                }

            } else if (!strcmp(command, "size")) {
                command = strtok(NULL, "");
                if (NULL == command) {
                    printf("size: %ld\n", treeSize(tree));
                } else {
                    currNode = findNode(tree, command);
                    printf("size: %ld\n", treeSize(currNode));
                }

            } else if (!strcmp(command, "height")) {
                command = strtok(NULL, "");
                if (NULL == command) {
                    currNode = tree;
                } else {
                    currNode = findNode(tree, command);
                }
                printf("height: %ld\n", (currNode ? treeHeight(currNode) : -1));

            } else if (!strcmp(command, "help")) {
                printf("%s\n", helpText);
            } else if (!strcmp(command, "init")) {
                freeTree(tree);
                free(tree);
                tree = NULL;
            } else if (!strcmp(command, "quit")) {
                break;

            } else if (!strcmp(command, "q")) {
                break;

            } else {
                fprintf(stderr, "'%s' is not a valid command\n", command);
            }
        }
    }
    free(buff);
    freeTree(tree);
    free(tree);
    return 0;
}
void processLine(treeNode **tree, char *buff) {
    char *parentName;
    char *childName;
    /* if there is a newline character, delete it */
    if (buff[strlen(buff) - 1] == '\n')
        buff[strlen(buff) - 1] = 0;
    buff = trim(buff);              /* trim the buffer */
    parentName = strtok(buff, ","); /* tokenize the string */
    parentName = trim(parentName);
    if (!*tree)                         /* check if this is the root node */
        *tree = createTree(parentName); /* if so, create tree */
    /* loop through all children in the line */
    while ((childName = strtok(NULL, ","))) {
        childName = trim(childName);
        addChild(*tree, parentName, childName);
    }
}
