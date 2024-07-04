#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

queue_t* queue_create(queue_data_t capacity) {
	queue_t* rt = (queue_t*)malloc(sizeof(queue_t));
	assert(rt != NULL);
	rt->array = (queue_data_t*)calloc(capacity, sizeof(queue_data_t));
	assert(rt->array != NULL);
	rt->capacity = capacity;
	rt->first = rt->size = 0;

	return rt;
}

void enqueue(queue_t* queue, queue_data_t value) {
	assert(queue->size < queue->capacity);  // capacité de queue est depasé
	const int idx_ajout = (queue->first + queue->size) % queue->capacity;
	queue->array[idx_ajout] = value;
	queue->size++;
}

queue_data_t dequeue(queue_t* queue) {
	assert(queue->size > 0);  // defilation de queue vide
	queue_data_t rt = queue->array[queue->first];
	queue->first = (queue->first + 1) % queue->capacity;
	queue->size--;

	return rt;
}

queue_t* queue_copy(const queue_t* target) {
	queue_t* rt = (queue_t*)malloc(sizeof(queue_t));
	assert(rt != NULL);
	rt->array = (queue_data_t*)calloc(target->capacity, sizeof(queue_data_t));
	assert(rt->array != NULL);

	rt->capacity = target->capacity;
	rt->first = target->first;
	rt->size = target->size;

	memcpy(rt->array, target->array, rt->capacity);

	return NULL;
}

void queue_free(queue_t* queue) {
	free(queue->array);
	free(queue);
}

bool queue_is_empty(queue_t* queue) {
	return queue->size == 0;
}

void queue_print(queue_t* queue) {
	for (int i = 0; i < queue->size; i++) {
		int idx = (i + queue->first) % queue->capacity;
		printf("%lld", queue->array[idx]);
		if (i != queue->size - 1)
			printf(", ");
	}
	printf("\n");
}
