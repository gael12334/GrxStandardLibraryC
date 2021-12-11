#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define grxDequeAllocSize 10
#define grxAllocDequeFront 0
#define grxAllocDequeBack 1

typedef struct grxDeque {
	void**		_data;			///<summary> Deque's data array.				</summary>
	uint32_t	_elemCount;		///<summary> Deque's element count.				</summary>
	uint32_t	_elemAlloc;		///<summary> Deque's allocated element count.	</summary>
	uint32_t	_frontIndex;	///<summary> Deque's front element index.		</summary>
	uint32_t	_backIndex;		///<summary> Deque's back element index.		</summary>
} grxDeque;

grxDeque* grxCreateDeque() {
	grxDeque* deque = (grxDeque*)malloc(sizeof(grxDeque));
	deque->_data = NULL;
	deque->_elemCount = 0;
	deque->_elemAlloc = 0;
	deque->_frontIndex = 0;
	deque->_backIndex = 0;
	return deque;
}

void grxFreeDeque(grxDeque** _dequePtr) {
	free((*_dequePtr)->_data);
	free(*_dequePtr);
	*_dequePtr = NULL;
}

/// <summary>
/// THIS IS AN INTERNAL FUNCTION. DO NOT USE.
/// </summary>
/// <param name="_deque">: The working deque instance.</param>
/// <param name="_bOrF">: Position of allocation of memory.</param>
void grxAllocDeque(grxDeque* _deque, int8_t _bOrF) {
	if (_deque->_elemAlloc == 0U) {
		_deque->_data		= (void**)calloc(grxDequeAllocSize, sizeof(void*));
		_deque->_elemAlloc	= grxDequeAllocSize;
		_deque->_elemCount	= 0U;
		_deque->_frontIndex	= (_bOrF == grxAllocDequeFront) ? grxDequeAllocSize - 1 : 0;
		_deque->_backIndex	= (_bOrF == grxAllocDequeFront) ? grxDequeAllocSize - 1 : 0;
	}

	else if (_bOrF == grxAllocDequeFront) {
		if (_deque->_frontIndex == 0) {
			void** dequeArray = (void**)calloc(_deque->_elemAlloc + grxDequeAllocSize, sizeof(void*));
			
			uint32_t _dequeAllocatedBytes = sizeof(void*) * _deque->_elemCount;
			memcpy_s(&dequeArray[grxDequeAllocSize - 1], _dequeAllocatedBytes, _deque->_data, _dequeAllocatedBytes);
			
			free(_deque->_data);
			_deque->_data = dequeArray;
			_deque->_elemAlloc += grxDequeAllocSize;
			_deque->_backIndex += grxDequeAllocSize;
			_deque->_frontIndex = grxDequeAllocSize - 1;
			_deque->_elemCount++;
		}
		else {
			_deque->_frontIndex--;
			_deque->_elemCount++;
		}
	}

	else {
		if (_deque->_backIndex == (_deque->_elemAlloc - 1)) {
			void** dequeArray = (void**)calloc(_deque->_elemAlloc + grxDequeAllocSize, sizeof(void*));

			uint32_t _dequeAllocatedBytes = sizeof(void*) * _deque->_elemCount;
			memcpy_s(dequeArray, _dequeAllocatedBytes, _deque->_data, _dequeAllocatedBytes);

			free(_deque->_data);
			_deque->_data = dequeArray;
			_deque->_elemAlloc += grxDequeAllocSize;
			_deque->_backIndex++;
			_deque->_elemCount++;
		}
		else {
			_deque->_backIndex++;
			_deque->_elemCount++;
		}
	}
}

/// <summary>
/// THIS IS AN INTERNAL FUNCTION. DO NOT USE.
/// </summary>
/// <param name="_deque">: The working deque instance.</param>
/// <param name="_bOrF">: Position of allocation of memory.</param>
/// <returns>Returns the success of the operation.</returns>
int8_t grxShrinkDeque(grxDeque* _deque, int8_t _bOrF) {

	return 0;
}

void grxAddDequeFront(grxDeque* _deque, void* _value) {
	grxAllocDeque(_deque, grxAllocDequeFront);
	_deque->_data[_deque->_frontIndex] = _value;
}

void grxAddDequeBack(grxDeque* _deque, void* _value) {
	grxAllocDeque(_deque, grxAllocDequeBack);
	_deque->_data[_deque->_backIndex] = _value;
}






#undef grxDequeAllocSize
#undef grxAllocDequeFront 
#undef grxAllocDequeBack 