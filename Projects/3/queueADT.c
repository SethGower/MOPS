/******************************************************************************
 * File:             queueADT.c
 *
 * Author:           Seth Gower
 * Created:          11/17/19
 *                   Queue ADT implementation
 *****************************************************************************/
#include "queueADT.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 10
#define GROWTH_SIZE 10

typedef struct entry_s {
    struct entry_s *next;
    void *data;
} entry_t;
struct queueStruct {
    entry_t *front;
    entry_t *back;
    size_t numElements;
    bool (*equiv)(void *item1, void *item2);
};

queue_t que_create() {
    queue_t q = (queue_t)malloc(sizeof(struct queueStruct));
    q->front = q->back = NULL;
    q->numElements = 0;
    return q;
}
void que_destroy(queue_t q) {
    que_clear(q);
    free(q);
}
void que_clear(queue_t q) {
    while (q->numElements)
        que_dequeue(q);
}
void que_enqueue(queue_t q, void *data) {
    assert(NULL != q);
    entry_t *newNode = (entry_t *)malloc(sizeof(entry_t));
    newNode->next = NULL;
    newNode->data = data;
    if (NULL != q->back) {
        q->back->next = newNode;
        q->back = newNode;
    } else {
        q->back = q->front = newNode;
    }
    q->numElements++;
}
void que_dequeue(queue_t q) {
    assert(NULL != q);
    void *data;
    entry_t *oldFront;
    if (NULL != q->front) {
        data = q->front->data;
        oldFront = q->front;
        q->front = q->front->next;
        free(oldFront->data);
        free(oldFront);
        q->numElements--;
    }
}
const void *que_front(const queue_t q) {
    if (q->front)
        return q->front->data;
    else
        return NULL;
}
size_t que_size(queue_t q) {
    assert(NULL != q);
    return q->numElements;
}
