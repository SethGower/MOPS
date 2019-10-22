#include "symtab.h"
#include "trimit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static symbol_t *table = NULL;
void build_table(char *filename) {
    /* opens file to read in symbols */
    FILE *file = fopen(filename, "r");
    char *buff = malloc(BUFLEN); /* string to store the current line */
    char *tokens;                /* string for the current token */
    int i;                       /* loop control variable for for loops */
    char currSym[BUFLEN];        /* current symbol being added to table */
    int currVal;        /* value of current symbol being added to table*/
    if (NULL != file) { /* checks if the file exists */
        while (fgets(buff, BUFLEN, file)) { /* loop every line */
            buff = trim(buff);  /* trims trailing and leading whitespace */
            if (buff[0] == '#') /* check if comment */
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
            /* get the symbol name from line */
            tokens = strtok(buff, " ");
            strcpy(currSym, tokens);
            /* get the value */
            tokens = strtok(NULL, " ");
            currVal = strtol(tokens, NULL, 10); /* convert to int */
            /* create symbol using this value and symbol name */
            create_symbol(currSym, currVal);
        }
    } else { /* file was unable to be opened */
        fprintf(stderr, "Error opening symbol table file. %s:%d\n", __FILE__,
                __LINE__);
        exit(EXIT_FAILURE);
    }
    free(buff);   /* frees the string buffer */
    fclose(file); /* close the file */
}

void dump_table() {
    /* if the symbol table is empty, just return without printing */
    if (!table)
        return;
    symbol_t *currNode = table; /* temp node for traversal */
    printf("SYMBOL TABLE:\n");
    /* traverse the table */
    while (currNode) {
        printf("    Symbol: %s, Value: %d\n", currNode->var_name,
               currNode->val);
        currNode = currNode->next;
    }
}

symbol_t *lookup_table(char *variable) {
    symbol_t *currNode = table; /* temp node for traversal */
    while (currNode) {          /* traverse, looking for sym */
        if (!strcmp(variable, currNode->var_name)) /* if equal */
            return currNode;
        currNode = currNode->next; /* if not equal, go to next node */
    }
    /* if it has gotten to this point, then the variable wasn't in the table */
    return NULL;
}
/* creates a symbol in the table, and inserts it into the table alphabetically
 */
symbol_t *create_symbol(char *name, int val) {
#ifdef DEBUG /* if compiled with DEBUG, then print every time symbol is        \
                created */
    printf("Adding symbol: %s = %d to table\n", name, val);
#endif
    /* create new node on heap */
    symbol_t *newNode = malloc(sizeof(symbol_t));
    symbol_t *currNode = table; /* start at beginning of table */
    symbol_t *prevNode = NULL;
    if (currNode == NULL) { /* if the first node being added */
        table = newNode;
        table->var_name = malloc(strlen(name) + 1); /* create space for name */
        strcpy(table->var_name, name);              /* copy in name */
        table->val = val;                           /* direct copy the value */
        table->next = NULL;                         /* the next thing is NULL */
    } else {
        /* iterate through the linked list, until either the end of the list
         * has been reached or it has found something that comes before it
         * alphabetically
         *
         * - currNode == NULL: This happens if the symbol is lexiographically
         *   last
         * - strcmp() <= 0: This happens when the thing is before the current
         *   symbol being checked alphabetically*/
        while (currNode && strcmp(name, currNode->var_name) > 0) {
            prevNode = currNode;
            currNode = currNode->next;
        }
        /* If the node before the current node is NULL, don't access it */
        if (!prevNode) {
            table = newNode;
            newNode->next = currNode;
        } else {
            prevNode->next = newNode;
            newNode->next = currNode;
        }
        /* sets the various attributes of the new node */
        currNode = newNode;
        /* allocates enough space for just the string and the null terminator */
        currNode->var_name = malloc(strlen(name) + 1);
        strcpy(currNode->var_name, name);
        currNode->val = val;
    }
    /* return the address to the new node that was created */
    return currNode;
}

void free_table() {
    /* sets up temp nodes for list traversal */
    symbol_t *currNode = table;
    symbol_t *nextNode = NULL;
    /* loop while not NULL */
    while (currNode) {
        free(currNode->var_name);  /* free the name string */
        nextNode = currNode->next; /* setup next node for traversal */
        free(currNode);            /* free current node */
        currNode = nextNode;       /* go to next node */
    }
}
