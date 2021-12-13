#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct grxPair {
	void* _key;
	void* _entry;
} grxPair;

grxPair* grxCreatePair(void* _key, void* _entry) {
	grxPair* pair = (grxPair*)malloc(sizeof(grxPair));
	pair->_key = _key;
	pair->_entry = _entry;
	return pair;
}

void grxFreePair(grxPair** _pair) {
	free(*_pair);
	*_pair = NULL;
}