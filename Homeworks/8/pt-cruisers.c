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
    pthread_t *threads = NULL;
    racer_t **racers = NULL;
    int firstRacer = 2;
    int i = 0;
    int numRacers = argc - 2;
    int rc = 0;
    void *retVal;
    bool invalidName = false;
    char *paddedName = malloc(MAX_NAME_LEN + 1);
    paddedName[MAX_NAME_LEN] = '\0';
    int requiredArgs = 4;

    if (argc < 3) {
        fprintf(stderr,
                "Usage: pt-cruisers [ delay ] racer1 racer2 [racer3...]\n");
    } else {
        delay = strtol(argv[1], &strtolPtr, 10);
        if (strtolPtr == argv[1]) { /* no delay was passed in */
            delay = DEFAULT_WAIT;
            firstRacer = 1;
            numRacers = argc - 1;
            requiredArgs = 3;
        }
        if (delay > 0 && argc >= requiredArgs) {
            threads = calloc(numRacers, sizeof(pthread_t));
            racers = calloc(numRacers, sizeof(racer_t *));
            init_racers(delay);
            for (i = 0; i < numRacers; i++) {
                if (strlen(argv[i + firstRacer]) > MAX_NAME_LEN) {
                    invalidName = true;
                    break;
                }
                padName(argv[i + firstRacer], paddedName);
                racers[i] = make_racer(paddedName, i);
            }
            if (!invalidName) {
                for (i = 0; i < numRacers; i++) {
                    rc = pthread_create(&threads[i], NULL, run,
                                        (void *)racers[i]);
                    if (rc) {
                        fprintf(stderr, "ERROR: pthread_create returned %d\n",
                                rc);
                        break;
                    }
                }
                if (!rc) {
                    clear();
                    for (i = 0; i < numRacers; i++) {
                        pthread_join(threads[i], &retVal);
                    }
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
    if (threads)
        free(threads);
    if (racers) {
        for (i = 0; i < numRacers; i++) {
            destroy_racer(racers[i]);
        }
        free(racers);
    }
    free(paddedName);

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
