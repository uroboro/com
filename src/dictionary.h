#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "cobject.h"
#include "container.h"

CO_Struct(Dictionary) {
	void *data;
	int size;			// size of data
	int n;				// elements in data

	// Member functions
	Type (*retainer)(Type);
	void (*releaser)(Type);
	String *(*descriptor)(Type);

	Type (*get)(Dictionary *, const String *key);
	Dictionary *(*set)(Dictionary *, const String *key, Type);
	Dictionary *(*unset)(Dictionary *, const String *key);

	String *(*description)(Dictionary *);

	void (*dealloc)(Dictionary *);
};

Dictionary *DictionaryAlloc(void);

#endif /* DICTIONARY_H */
