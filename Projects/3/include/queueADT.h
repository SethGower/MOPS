#ifndef _QUEUEADT_H_
#define _QUEUEADT_H_

#include <stdbool.h>
#include <unistd.h>

typedef struct queueStruct *queue_t;

/******************************************************************************
 * Function:         queue_t que_create
 *                   Creates an empty queue
 * Return:           Pointer to the queue_t object
 * Error:            None
 *****************************************************************************/
queue_t que_create();

/******************************************************************************
 * Function:         void que_destroy
 *                   Frees all memory associated with a queue
 * Where:
 *                   queue_t queue - queue to be destroyed
 * Return:           None
 * Error:            None
 *****************************************************************************/
void que_destroy(queue_t queue);

/******************************************************************************
 * Function:         void que_clear
 *                   Dequeus all elements form the queue
 * Where:
 *                   queue_t queue - queue to be cleared
 * Return:           None
 * Error:
 *****************************************************************************/
void que_clear(queue_t queue);

/******************************************************************************
 * Function:         void que_enqueue
 *                   Enques the data into the queue
 * Where:
 *                   queue_t queue - queue to add to
 *                   void *data - a pointer to the data that is being added
 * Return:           None
 * Error:
 *****************************************************************************/
void que_enqueue(queue_t queue, void *data);

/******************************************************************************
 * Function:         void *que_dequeue
 * Description:      Dequeues an item from the queue and returns the item that
 *                   was removed
 * Where:
 *                   queue_t queue - queue from which to remove an item
 * Return:           Pointer to the data that was removed
 * Error:            None
 *****************************************************************************/
void *que_dequeue(queue_t queue);

/******************************************************************************
 * Function:         void *que_front
 * Description:      Returns the pointer that is currently at the front of the
 *                   queue. Same thing as dequeue just doesn't actually remove
 *                   it
 * Where:
 *                   const queue_t queue - queue to check
 * Return:           Pointer to the item at the front
 * Error:            None
 *****************************************************************************/
void *que_front(const queue_t queue);

/******************************************************************************
 * Function:         size_t que_size
 *                   Returns the number of items in the queue
 * Where:
 *                   queue_t queue - queue of which to return the size
 * Return:           Number of items currently in the queue
 * Error:
 *****************************************************************************/
size_t que_size(queue_t queue);

#endif
