#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct grxStackNode {
	struct grxStackNode* _prev;
	void* _data;
} grxStackNode;

grxStackNode* grxCreateStackNode(grxStackNode* _prev, void* _value) {
	grxStackNode* node = (grxStackNode*)malloc(sizeof(grxStackNode));
	node->_prev = _prev;
	node->_data = _value;
	return node;
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxStackIterator {
	grxStackNode* _node;
} grxStackIterator;

int8_t grxHasStackIteratorPrev(grxStackIterator* _it) {
	return _it->_node != NULL;
}

void* grxGetStackIteratorValue(const grxStackIterator* _it) {
	return (_it->_node == NULL) ? NULL : _it->_node->_data;
}

int8_t grxStackIteratorPrev(grxStackIterator* _it) {
	if (_it->_node) {
		_it->_node = _it->_node->_prev;
		return 1U;
	}
	else
		return 0U;
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxStack {
	grxStackNode* _top;
	uint32_t _count;
} grxStack;

grxStack* grxCreateStack() {
	grxStack* stack = (grxStack*)calloc(1, sizeof(grxStack));
	stack->_top = NULL;
	stack->_count = 0U;

	return stack;
}

void grxFreeStack(grxStack** _stackPtr) {
	grxStackNode* node = (*_stackPtr)->_top;
	grxStackNode* temp = NULL;
	
	while (node->_prev) {
		temp = node->_prev;
		free(node);
		node = temp;
	}
	free(node);
	free(*_stackPtr);
	*_stackPtr = NULL;
}

void grxPushStack(grxStack* _stack, void* _value) {
	if (_stack->_top == NULL) 
		_stack->_top = grxCreateStackNode(NULL, _value);
	else 
		_stack->_top = grxCreateStackNode(_stack->_top, _value);

	_stack->_count++;
}

void grxPopStack(grxStack* _stack) {
	if (_stack->_top == NULL)
		return;
	else {
		grxStackNode* node = _stack->_top;
		_stack->_top = _stack->_top->_prev;
		_stack->_count--;
		free(node);
	}
}

uint32_t grxGetStackCount(const grxStack* _stack) {
	return _stack->_count;
}

void* grxGetStackTopValue(const grxStack* _stack) {
	return (_stack->_top == NULL) ? NULL : _stack->_top->_data;
}

grxStackIterator grxGetStackIterator(const grxStack* _stack) {
	return { _stack->_top };
}