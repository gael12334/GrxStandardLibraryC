#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
* front                   back
* 	↓                       ↓
*  [ ] → [ ] → [ ] → [ ] → [ ] 
*/

typedef struct grxQueueNode {
	struct grxQueueNode* _next;
	void* _data;
} grxQueueNode;

grxQueueNode* grxCreateQueueNode(void* _value) {
	grxQueueNode* node = (grxQueueNode*)calloc(1, sizeof(grxQueueNode));
	node->_data = _value;
	return node;
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxQueueIterator {
	grxQueueNode* _node;
} grxQueueIterator;

int8_t grxHasQueueIteratorNext(grxQueueIterator* _it) {
	return _it->_node != NULL;
}

void* grxGetQueueIteratorValue(const grxQueueIterator* _it) {
	return (_it->_node == NULL) ? NULL : _it->_node->_data;
}

int8_t grxQueueIteratorNext(grxQueueIterator* _it) {
	if (_it->_node) {
		_it->_node = _it->_node->_next;
		return 1U;
	}
	else
		return 0U;
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxQueue {
	grxQueueNode* _front;
	grxQueueNode* _back;
	uint32_t _count;
} grxQueue;

grxQueue* grxCreateQueue() {
	grxQueue* queue = (grxQueue*)calloc(1, sizeof(grxQueue));
	return queue;
}

void grxPushQueue(grxQueue* _queue, void* _value) {
	if (_queue->_back == NULL)
		_queue->_front = _queue->_back = grxCreateQueueNode(_value);
	else
		_queue->_back = _queue->_back->_next = grxCreateQueueNode(_value);
	
	_queue->_count++;
}

void grxPopQueue(grxQueue* _queue) {
	if (_queue->_front == NULL)
		return;

	grxQueueNode* temp = _queue->_front;
	_queue->_front = _queue->_front->_next;
	_queue->_count--;

	free(temp);
}

uint32_t grxGetQueueCount(const grxQueue* _queue) {
	return _queue->_count;
}

void grxFreeQueue(grxQueue** _queuePtr) {
	while (grxGetQueueCount(*_queuePtr))
		grxPopQueue(*_queuePtr);
	free(*_queuePtr);
	*_queuePtr = NULL;
}

void* grxGetQueueFrontValue(const grxQueue* _queue) {
	return _queue->_front->_data;
}

void* grxGetQueueBackValue(const grxQueue* _queue) {
	return _queue->_back->_data;
}

grxQueueIterator grxGetQueueIterator(const grxQueue* _queue) {
	return { _queue->_front };
}