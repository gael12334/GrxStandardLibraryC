#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct grxListNode {
	struct grxListNode* _prev;
	struct grxListNode* _next;
	void* _data;
} grxListNode;

grxListNode* grxCreateListNode(grxListNode* _prev, grxListNode* _next, void* _data) {
	grxListNode* node = (grxListNode*)calloc(1, sizeof(grxListNode));
	node->_prev = _prev;
	node->_next = _next;
	node->_data = _data;
	return node;
}

// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxListIterator {
	grxListNode* _node;
} grxListIterator;

int8_t grxHasListIteratorPrev(grxListIterator* _it) {
	return _it->_node != NULL;
}

int8_t grxHasListIteratorNext(grxListIterator* _it) {
	return _it->_node != NULL;
}

void* grxGetListIteratorValue(const grxListIterator* _it) {
	return (_it->_node == NULL) ? NULL : _it->_node->_data;
}

int8_t grxListIteratorPrev(grxListIterator* _it) {
	if (_it->_node) {
		_it->_node = _it->_node->_prev;
		return 1U;
	}
	else
		return 0U;
}

int8_t grxListIteratorNext(grxListIterator* _it) {
	if (_it->_node) {
		_it->_node = _it->_node->_next;
		return 1U;
	}
	else
		return 0U;
}


// = = = = = = = = = = = = = = = = = = = = = = = =

typedef struct grxList {
	grxListNode* _front;
	grxListNode* _back;
	uint32_t _count;
} grxList;

grxList* grxCreateList() {
	grxList* list = (grxList*)calloc(1, sizeof(grxList));
	return list;
}

void grxAddListFront(grxList* _list, void* _value) {
	if (_list->_front == NULL)
		_list->_front = _list->_back = grxCreateListNode(NULL, NULL, _value);
	else
		_list->_front = _list->_front->_prev = grxCreateListNode(NULL, _list->_front, _value);

	_list->_count++;
}

void grxAddListBack(grxList* _list, void* _value) {
	if (_list->_back == NULL)
		_list->_back = _list->_front = grxCreateListNode(NULL, NULL, _value);
	else
		_list->_back = _list->_back->_next = grxCreateListNode(_list->_back, NULL, _value);
	_list->_count++;
}

void grxRemoveListFront(grxList* _list) {
	if (_list->_front == NULL)
		return;
	else if (_list->_front == _list->_back) {
		free(_list->_front);
		_list->_front = NULL;
		_list->_back = NULL;
		_list->_count = 0U;
	}
	else {
		grxListNode* node = _list->_front;
		_list->_front = _list->_front->_next;
		_list->_count--;
		free(node);
	}
}

void grxRemoveListBack(grxList* _list) {
	if (_list->_back == NULL)
		return;
	else if (_list->_front == _list->_back) {
		free(_list->_front);
		_list->_front = NULL;
		_list->_back = NULL;
		_list->_count = 0U;
	}
	else {
		grxListNode* node = _list->_back;
		_list->_back = _list->_back->_prev;
		_list->_count--;
		free(node);
	}
}

uint32_t grxGetListCount(const grxList* _list) {
	return _list->_count;
}

void grxFreeList(grxList** _list) {
	while (grxGetListCount(*_list)) 
		grxRemoveListBack(*_list);
	free(*_list);
	*_list = NULL;
}

void* grxGetListFrontValue(const grxList* _list) {
	return _list->_front->_data;
}

void* grxGetListBackValue(const grxList* _list) {
	return _list->_back->_data;
}

grxListIterator grxGetListFrontIterator(const grxList* _list) {
	return { _list->_front };
}

grxListIterator grxGetListBackIterator(const grxList* _list) {
	return { _list->_back };
}

grxListIterator grxGetListIterator(const grxList* _list, uint32_t _index) {
	if (_index < _list->_count) {
		grxListIterator it = grxGetListFrontIterator(_list);
		for (uint32_t i = 0U; i < _index; i++)
			grxListIteratorNext(&it);
		return it;
	}

	return { NULL };
}

void grxListAdd(grxList* _list, grxListIterator* _it, void* _value) {
	if (_it->_node == NULL) {
		grxAddListFront(_list, _value);
		*_it = grxGetListFrontIterator(_list);
	}

	grxListNode* node = grxCreateListNode(_it->_node->_prev, _it->_node, _value);
	
	if (_it->_node->_prev)
		_it->_node->_prev->_next = node;
	if (_it->_node->_next)
		_it->_node->_next->_prev = node;

	_list->_count++;
}

void grxListRemove(grxList* _list, grxListIterator* _it) {
	if ((_it->_node == NULL) || (_it->_node == _list->_front)) {
		grxRemoveListFront(_list);
		*_it = grxGetListFrontIterator(_list);
	}

	grxListNode* temp = _it->_node;
	if (_it->_node->_next)
		_it->_node->_next->_prev = _it->_node->_prev;
	if (_it->_node->_prev)
		_it->_node->_prev->_next = _it->_node->_next;

	_it->_node = _it->_node->_next;
	_list->_count--;
	free(temp);
}