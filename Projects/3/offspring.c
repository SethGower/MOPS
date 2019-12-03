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
    char *buff = malloc(MAX_BUF_SIZE); /* buffer to store strings */
    size_t buffSize = MAX_BUF_SIZE;    /* variable to pass to getline */
    treeNode *tree = NULL;
    size_t i = 0;
    const char *prompt = "offspring> "; /* promt text */
    bool running = true;
    /* help text for printing */
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
            fprintf(stderr, "error: could not open file '%s'\n", argv[1]);
        } else {
            /* loop through entire file, saving lines to buff */
            while (getline(&buff, &buffSize, fp) > 0) {
                processLine(&tree, buff);
            }
            fclose(fp);
        }
    }
    /* main command loop */
    do {
        printf("%s", prompt); /* print the prompt */
        /* if nothing is read in, then continue */
        if (getline(&buff, &buffSize, stdin) > 0) {
            if (buff[strlen(buff) - 1] == '\n')
                buff[strlen(buff) - 1] = 0;
            buff = trim(buff); /* trim the buffer */
            /* replace all tabs inside string with spaces. trim does this for
             * leading and trailing spaces and tabs, but doesn't for internal
             * ones. Internal ones will break strtok */
            for (i = 0; i < strlen(buff); i++) {
                if (buff[i] == '\t')
                    buff[i] = ' ';
            }
            if (!strlen(buff)) /* if the length is 0 after newline is stripped,
                                  that means that it was an empty line of input.
                                  Continue loop */
                continue;
            printf("+ %s\n\n", buff);    /* reprinting command */
            command = strtok(buff, " "); /* get the command that was passed */
            command = trim(command);     /* trim whitespace */
            /* this long chain of if statements simply finds which command was
             * run */
            if (!strcmp(command, "add")) {
                /* this line simply gets the remaining parts of the command
                 * (parent child list for add for example) */
                command = strtok(NULL, "");
                if (NULL == command || !strchr(buff, ',')) {
                    fprintf(stderr, "Usage: 'add parent name, child name'\n");
                    continue;
                }
                /* process the line that was read in */
                processLine(&tree, command);

            } else if (!strcmp(command, "find")) {
                command = strtok(NULL, "");
                if (NULL == command) /* if no optional command, set the string
                                        to search for to '' */
                    command = (tree ? tree->name : "");
                currNode = findNode(tree, command);
                if (NULL == currNode) { /* if nothing was found */
                    fprintf(stderr, "error: '%s' not found.\n",
                            (command ? command : ""));
                } else
                    printNode(currNode); /* if something was found, print that
                                            nodes direct descendants */

            } else if (!strcmp(command, "print")) {
                command = strtok(NULL, "");
                if (NULL == command) /* if no optional argument,
                                        print whole tree */
                    currNode = tree;
                else
                    currNode = findNode(tree, command);
                if (NULL == currNode) {
                    /* if the tree/node being searched for was NULL, then
                     * print something saying it wasn't found. If the command
                     * was not NULL (ie a name was passed in) print that. Else
                     * print an empty string*/
                    fprintf(stderr, "error: '%s' not found.\n",
                            (command ? command : ""));
                } else
                    /* if it was found, then print the whole tree starting
                     * there */
                    printTree(currNode);

            } else if (!strcmp(command, "size")) {
                command = strtok(NULL, "");
                if (NULL == command) { /* if no argument was passed in, print
                                          size of whole tree */
                    printf("size: %ld\n", treeSize(tree));
                } else { /* else print the size of the node of that name */
                    currNode = findNode(tree, command);
                    printf("size: %ld\n", treeSize(currNode));
                }

            } else if (!strcmp(command, "height")) {
                command = strtok(NULL, "");
                if (NULL == command) { /* same with height */
                    currNode = tree;
                } else {
                    currNode = findNode(tree, command);
                }
                printf("height: %d\n", (currNode ? treeHeight(currNode) : -1));

            } else if (!strcmp(command, "help")) {
                printf("%s\n", helpText);
            } else if (!strcmp(command, "init")) {
                freeTree(tree);
                free(tree);
                tree = NULL;
            } else if (!strcmp(command,
                               "quit")) { /* allows for both 'quit' and 'q' */
                break;

            } else if (!strcmp(command, "q")) {
                break;

            } else {
                fprintf(stderr, "'%s' is not a valid command\n", command);
            }
        } else {
            putchar('\n');
            running = false;
        }

    } while (running);
    /* free all memory */
    free(buff);
    freeTree(tree);
    free(tree);
    return 0;
}
/******************************************************************************
 * Function:         void processLine
 * Description:      Processes the line of parent and children in buff. Used
 *                   while reading in the file and when the 'add' command is
 *                   invoked
 * Where:
 *                   treeNode **tree - Pointer to the location of a pointer to
 *                   a tree object. This is so that it can modify the actual
 *                   pointer
 *                   char *buff - string containing the current line being
 *                   processed
 * Return:           None
 * Error:            None
 *****************************************************************************/
void processLine(treeNode **tree, char *buff) {
    char *parentName;
    char *childName;
    size_t i = 0;
    /* if there is a newline character, delete it */
    if (buff[strlen(buff) - 1] == '\n')
        buff[strlen(buff) - 1] = 0;
    buff = trim(buff); /* trim the buffer */
    /* replace all tabs inside string with spaces. trim does this for
     * leading and trailing spaces and tabs, but doesn't for internal
     * ones. Internal ones will break strtok */
    for (i = 0; i < strlen(buff); i++) {
        if (buff[i] == '\t')
            buff[i] = ' ';
    }
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
