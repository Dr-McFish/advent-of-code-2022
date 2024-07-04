#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>

/* --------------- Fixed Capacity Queue --------------- */
struct queue;
typedef int64_t queue_data_t;
typedef struct queue queue_t;

/** @brief creates/allocates empty queue 				*/
/** @param capacity  Maximum capacity for the new queue	*/
/** @return  pointer to the new que on the heap 		*/
queue_t* queue_create(queue_data_t capacity);

/** @brief returns a newly alocated copy of queue 	*/
/** @param target  queue to coppy 					*/
/** @return  pointer to new queue 					*/
queue_t* queue_copy(const queue_t* target);

/** @brief frees queue from memory 			*/
/** @param queue  pointer to que to free	*/
void queue_free(queue_t* quque);

/** @brief inserts element at the end of queue 	*/
/** @param quque  destination 					*/
/** @param value  element to insert 			*/
void enqueue(queue_t* quque, queue_data_t value);

/** @brief gets the element at the beging of queue, while removing it from queue 	*/
/** @param quque  destination 														*/
/** @return  dequeued element value 												*/
queue_data_t dequeue(queue_t* quque);

/** @brief check weather the queue is empty or not	*/
/** @param quque  destination						*/
/** @return  'true'  if the queue is empty			*/
/**        | 'false' otherwise						*/
bool queue_is_empty(queue_t* quque);

/** @brief prints queue to stdout	*/
/** @param quque  to print			*/
void queue_print(queue_t* queue);

// **************************************************************************************** //
// ************************************	Implementation ************************************ //
// **************************************************************************************** //

// ring buffer array based Queue

struct queue {
	queue_data_t* array;
	queue_data_t first;
	queue_data_t size;
	queue_data_t capacity;
};

#endif
