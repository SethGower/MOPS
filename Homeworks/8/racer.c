/******************************************************************************
 * File:             racer.c
 *
 * Author:           Seth Gower
 * Created:          12/05/19
 *                   Racer module file for the pt-cruisers homework
 *****************************************************************************/
#define _DEFAULT_SOURCE
#include "racer.h"
#include "display.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* max delay for racers */
static long delayMax = DEFAULT_WAIT;
/* mutex to prevent threads from writing to display at same time */
pthread_mutex_t lock;

void init_racers(long milliseconds) {
    delayMax = milliseconds;
    pthread_mutex_init(&lock, NULL);
}
racer_t *make_racer(char *name, int position) {
    racer_t *racer = calloc(1, sizeof(racer_t)); /* racer variable */
    racer->row = position;                       /* set the row */
    racer->graphic = malloc(strlen(name) + 1);   /* allocate space for name */
    strcpy(racer->graphic, name);                /* copy over the name */
    racer->dist = 1; /* racer starts at starting line */
    return racer;
}
void destroy_racer(racer_t *racer) {
    if (NULL != racer) {
        free(racer->graphic);
        free(racer);
    }
}
void putRacer(racer_t *racer, int col) {
    /* the fact that the coordinates for the display are 1 indexed pissed me
     * off */
    size_t i = 0;
    set_cur_pos(racer->row + 1, col - 1);
    /* puts space at current space */
    put(' ');
    /* places each character in the name on the screen at new col */
    for (i = 0; i < strlen(racer->graphic); i++) {
        set_cur_pos(racer->row + 1, col + i);
        put(racer->graphic[i]);
    }
}
void *run(void *racer) {
    int lockResult;
    racer_t *temp = (racer_t *)racer;
    putRacer(temp, 1); /* place the race car at the starting line */
    long delay = 0;
    while (temp->dist <= FINISH_LINE) {
        delay = rand() % delayMax;
        usleep(delay * 1000); /* usleep takes microseconds, delay is in milli */
        /* locks the mutex to disable other threads from writing to display */
        lockResult = pthread_mutex_lock(&lock);
        if (lockResult) {
            perror("entering run function");
            return NULL;
        }
        /* draws the car to the display */
        putRacer(temp, ++temp->dist);
        /* unlocks the mutex */
        lockResult = pthread_mutex_unlock(&lock);
        if (lockResult) {
            perror("exiting run function");
            return NULL;
        }
    }
    return NULL;
}
