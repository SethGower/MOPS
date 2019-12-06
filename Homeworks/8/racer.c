#define _DEFAULT_SOURCE
#include "racer.h"
#include "display.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static long delayMax = DEFAULT_WAIT;

pthread_mutex_t lock;

void init_racers(long milliseconds) {
    delayMax = milliseconds;
    pthread_mutex_init(&lock, NULL);
}
racer_t *make_racer(char *name, int position) {
    racer_t *racer = calloc(1, sizeof(racer_t));
    racer->row = position;
    racer->graphic = malloc(strlen(name) + 1);
    strcpy(racer->graphic, name);
    racer->dist = 0;
    return racer;
}
void destroy_racer(racer_t *racer) {
    if (NULL != racer) {
        free(racer->graphic);
        free(racer);
    }
}
void putRacer(racer_t *racer, int col) {
    size_t i = 0;
    set_cur_pos(racer->row + 1, col - 1);
    put(' ');
    for (i = 0; i < strlen(racer->graphic); i++) {
        set_cur_pos(racer->row + 1, col + i);
        put(racer->graphic[i]);
    }
}
void *run(void *racer) {
    int lockResult;
    racer_t *temp = (racer_t *)racer;
    putRacer(temp, 1);
    long delay = 0;
    while (temp->dist <= FINISH_LINE) {
        /* lockResult = pthread_mutex_lock(&lock); */
        /* if (lockResult) { */
        /*     perror("entering run function"); */
        /*     return NULL; */
        /* } */
        delay = rand() % delayMax;
        usleep(delay * 1000);
        putRacer(temp, ++temp->dist);
        /* lockResult = pthread_mutex_unlock(&lock); */
        /* if (lockResult) { */
        /*     perror("exiting run function"); */
        /*     return NULL; */
        /* } */
    }
    return NULL;
}
