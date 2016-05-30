#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "cobject.h"

typedef void * Type;
typedef char String;

typedef Type (*DictionaryRetainer)(Type t);
typedef void (*DictionaryReleaser)(Type t);
typedef String *(*DictionaryDescriptor)(Type t);

CO_Struct(Dictionary) {
	int size;			/* size of the pointer table */
	int n;				/* elements in the pointer table */
	void *table;

	Type (*get)(Dictionary *, const char *key);

	Dictionary *(*set)(Dictionary *, const char *key, Type t);
	Type (*retainer)(Type t);
	void (*releaser)(Type t);

	String *(*description)(Dictionary *);
	String *(*descriptor)(Type t);

	void (*dealloc)(Dictionary *);
};

Dictionary *DictionaryAlloc(void);

#endif /* DICTIONARY_H */
