#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "deque.h"

#define grxStringAllocSize 10
#define grxAllocStringFront 0
#define grxAllocStringBack 1

typedef struct grxString {
	char*		  _data;       ///<summary> String's data array.				</summary>
	uint32_t	_elemCount;  ///<summary> String's element count.			</summary>
	uint32_t	_elemAlloc;  ///<summary> String's allocated element count.	</summary>
	uint32_t	_frontIndex; ///<summary> String's front element index.		</summary>
	uint32_t	_backIndex;	 ///<summary> String's back element index.		</summary>
} grxString;

void grxFreeString(grxString** _strPtr) {
	free((*_strPtr)->_data);
	free(*_strPtr);
	*_strPtr = NULL;
}

uint32_t grxGetStringCount(const grxString* _string) {
	return _string->_elemCount;
}

int8_t grxStringSwap(grxString* _string, uint32_t _posA, uint32_t _posB) {
	_posA += _string->_frontIndex;
	_posB += _string->_frontIndex;

	if (_posA > _string->_backIndex)
		return 0Ui8;

	if (_posB > _string->_backIndex)
		return 0Ui8;

	char valueA = _string->_data[_posA];
	_string->_data[_posA] = _string->_data[_posB];
	_string->_data[_posB] = valueA;
	return 1Ui8;
}

inline char grxGetStringValue(const grxString* _string, uint32_t _index) {
	return _string->_data[_string->_frontIndex + _index];
}

void grxSetStringValue(grxString* _string, uint32_t _index, char _value) {
	_string->_data[_string->_frontIndex + _index] = _value;
}

/// <summary>
/// THIS IS AN INTERNAL FUNCTION. DO NOT USE.
/// </summary>
/// <param name="_string">: The working String instance.</param>
/// <param name="_bOrF">: Position of allocation of memory.</param>
void grxAllocString(grxString* _string, int8_t _bOrF) {
	if (_string->_elemAlloc == 0U) {
		_string->_data = (char*)calloc(grxStringAllocSize, sizeof(char));
		_string->_elemAlloc = grxStringAllocSize;
		_string->_elemCount = 1U;
		_string->_frontIndex = (_bOrF == grxAllocStringFront) ? grxStringAllocSize - 1 : 0;
		_string->_backIndex = (_bOrF == grxAllocStringFront) ? grxStringAllocSize - 1 : 0;
	}

	else if (_bOrF == grxAllocStringFront) {
		if (_string->_frontIndex == 0) {
			char* strArray = (char*)calloc(_string->_elemAlloc + grxStringAllocSize, sizeof(char));

			uint32_t _strAllocatedBytes = sizeof(char) * _string->_elemAlloc;
			memcpy_s(&strArray[grxStringAllocSize], _strAllocatedBytes, _string->_data, _strAllocatedBytes);

			free(_string->_data);
			_string->_data = strArray;
			_string->_elemAlloc += grxStringAllocSize;
			_string->_backIndex += grxStringAllocSize;
			_string->_frontIndex = grxStringAllocSize - 1;
			_string->_elemCount++;
		}
		else {
			_string->_frontIndex--;
			_string->_elemCount++;
		}
	}

	else {
		if (_string->_backIndex == (_string->_elemAlloc - 2)) {
			char* strArray = (char*)calloc(_string->_elemAlloc + grxStringAllocSize, sizeof(char));

			uint32_t _strAllocatedBytes = sizeof(char) * _string->_elemAlloc;
			memcpy_s(strArray, _strAllocatedBytes, _string->_data, _strAllocatedBytes);

			free(_string->_data);
			_string->_data = strArray;
			_string->_elemAlloc += grxStringAllocSize;
			_string->_backIndex++;
			_string->_elemCount++;
		}
		else {
			_string->_backIndex++;
			_string->_elemCount++;
		}
	}
}

/// <summary>
/// THIS IS AN INTERNAL FUNCTION. DO NOT USE.
/// </summary>
/// <param name="_string">: The working String instance.</param>
/// <param name="_bOrF">: Position of allocation of memory.</param>
/// <returns>Returns the success of the operation.</returns>
int8_t grxShrinkString(grxString* _string, int8_t _bOrF) {
	if (_string->_elemAlloc == 0U)
		return 0;

	else if (_string->_elemCount == 1U) {
		free(_string->_data);
		_string->_data = NULL;
		_string->_elemAlloc = 0U;
		_string->_frontIndex = 0U;
		_string->_backIndex = 0U;
		return 1;
	}

	else {
		if (_bOrF == grxAllocStringFront) {
			if (grxStringAllocSize == (_string->_frontIndex + 1)) {
				char* strArray = (char*)calloc(_string->_elemAlloc - grxStringAllocSize, sizeof(char));

				uint32_t _strAllocatedBytes = _string->_elemAlloc - grxStringAllocSize;
				memcpy_s(strArray, _strAllocatedBytes, &_string->_data[_string->_frontIndex + 1], _strAllocatedBytes);
				free(_string->_data);
				_string->_data = strArray;

				_string->_backIndex -= grxStringAllocSize;
				_string->_frontIndex = 0U;
				_string->_elemCount--;
				_string->_elemAlloc -= grxStringAllocSize;
			}
			else {
				_string->_frontIndex++;
				_string->_elemCount--;
			}
		}

		else {
			if (grxStringAllocSize == (_string->_elemAlloc - _string->_backIndex - 1)) {
				char* strArray = (char*)calloc(_string->_elemAlloc - grxStringAllocSize, sizeof(char));

				uint32_t _strAllocatedBytes = _string->_elemAlloc - grxStringAllocSize;
				memcpy_s(strArray, _strAllocatedBytes, _string->_data, _strAllocatedBytes);
				free(_string->_data);
				_string->_data = strArray;

				_string->_backIndex--;
				_string->_elemCount--;
				_string->_elemAlloc -= grxStringAllocSize;
			}
			else {
				_string->_backIndex--;
				_string->_elemCount--;
			}
		}
	}

	return 0;
}

void grxAddStringFront(grxString* _string, char _value) {
	if (_value != '\0') {
		grxAllocString(_string, grxAllocStringFront);
		_string->_data[_string->_frontIndex] = _value;
	}
}

void grxAddStringBack(grxString* _string, char _value) {
	if (_value != '\0') {
		grxAllocString(_string, grxAllocStringBack);
		_string->_data[_string->_backIndex] = _value;
	}
}

void grxRemoveStringFront(grxString* _string) {
	if(grxGetStringCount(_string) > 1U)
		grxShrinkString(_string, grxAllocStringFront);
}

void grxRemoveStringBack(grxString* _string) {
	if (grxGetStringCount(_string) > 1U) {
		grxShrinkString(_string, grxAllocStringBack);
		grxSetStringValue(_string, _string->_backIndex, '\0');
	}
}

uint32_t grxSeekStringValue(const grxString* _string, char _value, int8_t* _failed) {
	for (uint32_t i = _string->_frontIndex, m = _string->_frontIndex + _string->_elemCount; i < m; i++) {
		if (_string->_data[i] == _value) {
			*_failed = 0U;
			return i;
		}
	}

	*_failed = 1U;
	return 0U;
}

grxString* grxCreateString() {
	grxString* str = (grxString*)calloc(1, sizeof(grxString));
	return str;
}

// =============
// string utility functions

void grxAssignString(grxString* _string, const char _text[]) {
	while (grxGetStringCount(_string));
		grxRemoveStringBack(_string);

	uint32_t textLength = strlen(_text) + 1;
	for (uint32_t i = 0U; i < textLength; i++) 
		grxAddStringBack(_string, _text[i]);
}

void grxReplaceStringChar(grxString* _string, char _seek, char _new) {
	if ((_seek == '\0') || (_new == '\0'))
		return;

	for (uint32_t i = _string->_frontIndex, m = i + _string->_elemCount; i < m; i++) {
		if (_string->_data[i] == _seek)
			_string->_data[i] = _new;
	}
}

const char* grxGetStringNative(grxString* _string) {
	return _string->_data + _string->_frontIndex;
}

int8_t grxStringContainsChar(grxString* _string, grxDeque** _deque, char _ch) {
	if (_deque == NULL)
		return 0;

	if (*_deque == NULL)
		*_deque = grxCreateDeque();

	else if (*_deque != NULL) {
		grxFreeDeque(_deque);
		*_deque = grxCreateDeque();
	}

	for (uint32_t i = _string->_frontIndex, m = i + _string->_elemCount; i < m; i++) {
		if (_string->_data[i] == _ch)
			grxAddDequeBack(*_deque, (void*)i);
	}

	return grxGetDequeCount(*_deque) ? 1 : 0;
}

grxString* grxCopyString(const grxString* _string) {
	grxString* copy = grxCreateString();
	copy->_data = (char*)malloc(_string->_elemAlloc);
	copy->_backIndex = _string->_backIndex;
	copy->_frontIndex = _string->_frontIndex;
	copy->_elemAlloc = _string->_elemAlloc;
	copy->_elemCount = _string->_elemCount;
	memcpy_s(copy->_data, copy->_elemAlloc, _string->_data, _string->_elemAlloc);

	return copy;
}

uint32_t grxFindString(const grxString* _string, grxString* _sequence, int8_t* result) {
	uint32_t seqCount = grxGetStringCount(_sequence);
	if (seqCount == 0U) {
		*result = 0U;
		return 0U;
	}

	char selectedCh;
	uint32_t seqIndex = 0U;
	for (uint32_t i = 0U, m = grxGetStringCount(_string); i < m; i++) {
		selectedCh = grxGetStringValue(_string, i);
		if (selectedCh == grxGetStringValue(_sequence, seqIndex)) {
			seqIndex++;
			if (seqIndex == seqCount) {
				*result = 1;
				return i - (seqCount - 1U);
			}
		}
			
		else
			seqIndex = 0U;
	}

	*result = 0;
	return 0U;
}

uint32_t grxFindStringNative(const grxString* _string, const char _text[], int8_t* result) {
	uint32_t seqCount = strlen(_text);
	if (seqCount == 0U) {
		*result = 0U;
		return 0U;
	}

	char selectedCh;
	uint32_t seqIndex = 0U;
	for (uint32_t i = 0U, m = grxGetStringCount(_string); i < m; i++) {
		selectedCh = grxGetStringValue(_string, i);
		if (selectedCh == _text[seqIndex]) {
			seqIndex++;
			if (seqIndex == seqCount) {
				*result = 1;
				return i - seqCount;
			}
		}

		else
			seqIndex = 0U;
	}

	*result = 0;
	return 0U;
}

int8_t grxIsStringEmpty(const grxString* _string) {
	return (grxGetStringCount(_string) ? 0U : 1U);
}

int8_t grxSplitString(const grxString* _string, grxString* _splitChs, grxDeque** _result) {
	if (_result == NULL)
		return 0;

	if (*_result == NULL)
		*_result = grxCreateDeque();

	else if (*_result != NULL) {
		grxFreeDeque(_result);
		*_result = grxCreateDeque();
	}

	uint32_t splitCount = grxGetStringCount(_splitChs);
	grxString* reciever = grxCreateString();
	char selectedChar;
	int8_t hasSplited = 0;

	for (uint32_t i = 0, m = grxGetStringCount(_string); i < m; i++) {
		selectedChar = grxGetStringValue(_string, i);
		for (uint32_t j = 0; j < splitCount; j++) {
			if (selectedChar == grxGetStringValue(_splitChs, j)) {
				grxAddDequeBack(*_result, reciever);
				reciever = grxCreateString();
				hasSplited = 1;
				break;
			}
		}

		if (!hasSplited)
			grxAddStringBack(reciever, selectedChar);
	}

	grxAddDequeBack(*_result, reciever);
	return 1;
}

int8_t grxSplitStringNative(const grxString* _string, const char _splitChs[], grxDeque** _result) {
	if (_result == NULL)
		return 0;

	if (*_result == NULL)
		*_result = grxCreateDeque();

	else if (*_result != NULL) {
		grxFreeDeque(_result);
		*_result = grxCreateDeque();
	}

	uint32_t splitCount = strlen(_splitChs) + 1U;
	grxString* reciever = grxCreateString();
	char selectedChar;
	int8_t hasSplited = 0;

	for (uint32_t i = 0, m = grxGetStringCount(_string); i < m; i++) {
		selectedChar = grxGetStringValue(_string, i);
		for (uint32_t j = 0; j < splitCount; j++) {
			if (selectedChar == _splitChs[j]) {
				grxAddDequeBack(*_result, reciever);
				reciever = grxCreateString();
				hasSplited = 1;
				break;
			}
		}

		if (!hasSplited)
			grxAddStringBack(reciever, selectedChar);
		else
			hasSplited = 0;
	}

	grxAddDequeBack(*_result, reciever);
	return 1;
}

void grxStringJoinBack(grxString* _string, const grxString* _toJoin) {
	char selected;

	for (uint32_t i = 0U, m = grxGetStringCount(_toJoin); i < m; i++) {
		selected = grxGetStringValue(_toJoin, i);
		grxAddStringBack(_string, selected);
	}
}

void grxStringJoinFront(grxString* _string, const grxString* _toJoin) {
	char selected;

	for (uint32_t i = grxGetStringCount(_toJoin) + 1; i != 1U; i--) {
		selected = grxGetStringValue(_toJoin, i - 2);
		grxAddStringFront(_string, selected);
	}
}

grxString* grxGetSubString(grxString* _string, uint32_t _fromIndex, uint32_t _toIndex) {
	grxString* sub = grxCreateString();
	uint32_t strCount = grxGetStringCount(_string);

	if ((_fromIndex >= strCount) || (_toIndex >= strCount))
		return NULL;
	
	char selected;
	for (uint32_t i = _fromIndex; i <= _toIndex; i++) {
		selected = grxGetStringValue(_string, i);
		grxAddStringBack(sub, selected);
	}

	return sub;
}

#undef grxStringAllocSize
#undef grxAllocStringFront 
#undef grxAllocStringBack 
