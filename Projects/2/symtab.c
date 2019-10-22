#include "symtab.h"
#include "trimit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static symbol_t *table = NULL;
void build_table(char *filename) {
    FILE *file = fopen(filename, "r");
    char *buff = malloc(BUFLEN);
    char *tokens;
    int i;
    char currSym[BUFLEN];
    int currVal;
    if (NULL != file) {
        while (fgets(buff, BUFLEN, file)) {
            buff = trim(buff);
            if (buff[0] == '#')
                continue;
            /* check if first character is a letter */
            if (!((buff[0] >= 65 && buff[0] <= 90) ||
                  (buff[0] >= 97 && buff[0] <= 122)))
                continue;
            /* replace all internal tabs to spaces */
            for (i = 0; i < (int)strlen(buff); i++) {
                if (buff[i] == '\t')
                    buff[i] = ' ';
            }
            tokens = strtok(buff, " ");
            strcpy(currSym, tokens);
            tokens = strtok(NULL, " ");
            currVal = strtol(tokens, NULL, 10);
            create_symbol(currSym, currVal);
        }
    } else {
        fprintf(stderr, "Error opening symbol table file. %s:%d\n", __FILE__,
                __LINE__);
        exit(EXIT_FAILURE);
    }
    free(buff);
    fclose(file);
}

void dump_table() {
    symbol_t *currNode = table;
    printf("SYMBOL TABLE:\n");
    while (currNode) {
        printf("    Symbol: %s, Value: %d\n", currNode->var_name,
               currNode->val);
        currNode = currNode->next;
    }
}

symbol_t *lookup_table(char *variable) {
    symbol_t *currNode = table;
    while (currNode) {
        if (!strcmp(variable, currNode->var_name))
            return currNode;
        currNode = currNode->next;
    }
    /* if it has gotten to this point, then the variable wasn't in the table */
    return NULL;
}

symbol_t *create_symbol(char *name, int val) {
    printf("Adding symbol: %s = %d to table\n", name, val);
    symbol_t *newNode = malloc(sizeof(symbol_t));
    symbol_t *currNode = table;
    symbol_t *prevNode = NULL;
    if (currNode == NULL) {
        table = newNode;
        table->var_name = malloc(strlen(name) + 1);
        strcpy(table->var_name, name);
        table->val = val;
        table->next = NULL;
    } else {
        while (currNode && strcmp(name, currNode->var_name) > 0) {
            prevNode = currNode;
            currNode = currNode->next;
        }
        if (!prevNode) {
            table = newNode;
            newNode->next = currNode;
        } else {
            prevNode->next = newNode;
            newNode->next = currNode;
        }
        currNode = newNode;
        currNode->var_name = malloc(strlen(name) + 1);
        strcpy(currNode->var_name, name);
        currNode->val = val;
    }
    return currNode;
}

void free_table() {
    symbol_t *currNode = table;
    symbol_t *nextNode = NULL;
    while (currNode) {
        free(currNode->var_name);
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}
