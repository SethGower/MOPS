/******************************************************************************
 * File:             pt-cruisers.c
 *
 * Author:           Seth Gower
 * Created:          12/05/19
 *                   Main module file for the pt-cruisers homework
 *****************************************************************************/
#include "display.h"
#include "racer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void padName(char *name, char *paddedName);
int main(int argc, char *argv[]) {
    srand(time(NULL));
    long delay = 69;
    char *strtolPtr;
    pthread_t *threads = NULL; /* array for all of the threads */
    racer_t **racers = NULL;   /* array for each racer object */
    int firstRacer = 2; /* which argument the first racer is, defaults to as if
                           there is a delay */
    int i = 0;          /* generic loop control variable */
    int numRacers = argc - 2; /* number of racers. Defaults to if a delay is
                                 passed in */
    int rc = 0;               /* return code for thread calls */
    void *retVal;             /* useless garbage variable */
    bool invalidName = false; /* did the loop exit because an invalid name? */
    char *paddedName = malloc(MAX_NAME_LEN + 1); /* variable which has the name
                                                    with the underscores */
    int requiredArgs = 4; /* required number of arguments. Defaults to having a
                             delay passed in */

    if (argc < 3) {
        /* if too few arguments were passed in */
        fprintf(stderr,
                "Usage: pt-cruisers [ delay ] racer1 racer2 [racer3...]\n");
    } else {
        /* try converting the first argument to a number.  */
        delay = strtol(argv[1], &strtolPtr, 10);
        if (strtolPtr == argv[1]) { /* no delay was passed in */
            /* if no delay was passed in, then change all necessary values to
             * reflect this. */
            delay = DEFAULT_WAIT;
            firstRacer = 1;
            numRacers = argc - 1;
            requiredArgs = 3;
        }
        if (delay > 0 && argc >= requiredArgs) {
            threads = calloc(numRacers, sizeof(pthread_t));
            racers = calloc(numRacers, sizeof(racer_t *));
            init_racers(delay);
            /* create all of the racer objects */
            for (i = 0; i < numRacers; i++) {
                /* check if the name valid. else print error message */
                if (strlen(argv[i + firstRacer]) > MAX_NAME_LEN) {
                    invalidName = true;
                    break;
                }
                /* get the name with underscores */
                padName(argv[i + firstRacer], paddedName);
                /* create the racer object */
                racers[i] = make_racer(paddedName, i);
            }
            if (!invalidName) {
                /* create all of the threads for the individual racers */
                for (i = 0; i < numRacers; i++) {
                    /* passes in the run function as the start_routine, and
                     * the appropriate racer object */
                    rc = pthread_create(&threads[i], NULL, run,
                                        (void *)racers[i]);
                    if (rc) {
                        fprintf(stderr, "ERROR: pthread_create returned %d\n",
                                rc);
                        break;
                    }
                }
                if (!rc) {
                    /* clear the screen and start running the threads */
                    clear();
                    for (i = 0; i < numRacers; i++) {
                        pthread_join(threads[i], &retVal);
                    }
                    /* this is so the cursor is good for the shell prompt on
                     * exit */
                    set_cur_pos(numRacers + 1, 1);
                }
            } else {
                fprintf(stderr,
                        "Usage: racer names must not exceed length %d.\n",
                        MAX_NAME_LEN);
            }
        } else if (delay < 0) {
            fprintf(stderr, "Delay %ld is invalid.\n", delay);
            fprintf(stderr,
                    "Usage: pt-cruisers [ delay ] racer1 racer2 [racer3...]\n");
        } else {
            fprintf(stderr,
                    "Usage: pt-cruisers [ delay ] racer1 racer2 [racer3...]\n");
        }
    }
    /* checks if the arrays have been allocated, if so free them */
    if (threads)
        free(threads);
    if (racers) {
        for (i = 0; i < numRacers; i++) {
            destroy_racer(racers[i]);
        }
        free(racers);
    }
    free(paddedName); /* free the buffer for the processed name */

    return 0;
}

void padName(char *name, char *paddedName) {
    int i = 0;
    int numUnderscores = MAX_NAME_LEN - strlen(name);
    for (i = 0; i < numUnderscores / 2; i++) {
        paddedName[i] = '_';
    }
    strcpy(&(paddedName[numUnderscores / 2]), name);
    for (i = numUnderscores / 2 + strlen(name); i < MAX_NAME_LEN; i++) {
        paddedName[i] = '_';
    }
    paddedName[MAX_NAME_LEN] = '\0';
}
