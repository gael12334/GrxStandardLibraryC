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
		_deque->_elemCount	= 1U;
		_deque->_frontIndex	= (_bOrF == grxAllocDequeFront) ? grxDequeAllocSize - 1 : 0;
		_deque->_backIndex	= (_bOrF == grxAllocDequeFront) ? grxDequeAllocSize - 1 : 0;
	}

	else if (_bOrF == grxAllocDequeFront) {
		if (_deque->_frontIndex == 0) {
			void** dequeArray = (void**)calloc(_deque->_elemAlloc + grxDequeAllocSize, sizeof(void*));
			
			uint32_t _dequeAllocatedBytes = sizeof(void*) * _deque->_elemAlloc;
			memcpy_s(&dequeArray[grxDequeAllocSize], _dequeAllocatedBytes, _deque->_data, _dequeAllocatedBytes);
			
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

			uint32_t _dequeAllocatedBytes = sizeof(void*) * _deque->_elemAlloc;
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
	if (_deque->_elemAlloc == 0U)
		return 0;

	else if (_deque->_elemCount == 1U) {
		free(_deque->_data);
		_deque->_data = NULL;
		_deque->_elemAlloc = 0U;
		_deque->_frontIndex = 0U;
		_deque->_backIndex = 0U;
		return 1;
	}

	else {
		if (_bOrF == grxAllocDequeFront) {
			if (grxDequeAllocSize == (_deque->_frontIndex + 1)) {
				void** dequeArray = (void**)calloc(_deque->_elemAlloc - grxDequeAllocSize, sizeof(void*));

				uint32_t _dequeAllocatedBytes = _deque->_elemAlloc - grxDequeAllocSize;
				memcpy_s(dequeArray, _dequeAllocatedBytes, &_deque->_data[_deque->_frontIndex + 1], _dequeAllocatedBytes);
				free(_deque->_data);
				_deque->_data = dequeArray;

				_deque->_backIndex -= grxDequeAllocSize;
				_deque->_frontIndex = 0U;
				_deque->_elemCount--;
				_deque->_elemAlloc -= grxDequeAllocSize;
			}
			else {
				_deque->_frontIndex++;
				_deque->_elemCount--;
			}
		}
		
		else {
			if (grxDequeAllocSize == (_deque->_elemAlloc - _deque->_backIndex - 1)) {
				void** dequeArray = (void**)calloc(_deque->_elemAlloc - grxDequeAllocSize, sizeof(void*));

				uint32_t _dequeAllocatedBytes = _deque->_elemAlloc - grxDequeAllocSize;
				memcpy_s(dequeArray, _dequeAllocatedBytes, _deque->_data, _dequeAllocatedBytes);
				free(_deque->_data);
				_deque->_data = dequeArray;

				_deque->_backIndex--;
				_deque->_elemCount--;
				_deque->_elemAlloc -= grxDequeAllocSize;
			}
			else {
				_deque->_backIndex--;
				_deque->_elemCount--;
			}
		}
	}

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

void grxRemoveDequeFront(grxDeque* _deque) {
	grxShrinkDeque(_deque, grxAllocDequeFront);
}

void grxRemoveDequeBack(grxDeque* _deque) {
	grxShrinkDeque(_deque, grxAllocDequeBack);
}

void* grxGetDequeValue(const grxDeque* _deque, uint32_t _index) {
	return _deque->_data[_deque->_frontIndex + _index];
}

void grxSetDequeValue(grxDeque* _deque, uint32_t _index, void* _value) {
	_deque->_data[_deque->_frontIndex + _index] = _value;
}

uint32_t grxGetDequeCount(const grxDeque* _deque) {
	return _deque->_elemCount;
}

int8_t grxDequeSwap(grxDeque* _deque, uint32_t _posA, uint32_t _posB) {
	_posA += _deque->_frontIndex;
	_posB += _deque->_frontIndex;

	if (_posA > _deque->_backIndex)
		return 0Ui8;

	if (_posB > _deque->_backIndex)
		return 0Ui8;

	void* valueA = _deque->_data[_posA];
	_deque->_data[_posA] = _deque->_data[_posB];
	_deque->_data[_posB] = valueA;
	return 1Ui8;
}

uint32_t grxSeekDequeValue(grxDeque* _deque, void* _value, int8_t* _failed) {
	for (uint32_t i = _deque->_frontIndex, m = _deque->_frontIndex + _deque->_elemCount; i < m; i++) {
		if (_deque->_data[i] == _value) {
			*_failed = 0U;
			return i;
		}
	}

	*_failed = 1U;
	return 0U;
}

#undef grxDequeAllocSize
#undef grxAllocDequeFront 
#undef grxAllocDequeBack 
