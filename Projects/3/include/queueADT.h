#ifndef _QUEUEADT_H_
#define _QUEUEADT_H_

#include <stdbool.h>
#include <unistd.h>

typedef struct queueStruct *queue_t;

queue_t que_create();
void que_destroy(queue_t queue);
void que_clear(queue_t queue);
void que_enqueue(queue_t queue, void *data);
void que_dequeue(queue_t queue);
const void *que_front(const queue_t queue);
size_t que_size(queue_t queue);

#endif
