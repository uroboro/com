#ifndef ARRAY_H
#define ARRAY_H

#include "cobject.h"
#include "container.h"

CO_Struct(Array) {
	void * data;
	int size;			// size of data
	int n;				// elements in data

	// Member functions
	Type (*retainer)(Type);
	void (*releaser)(Type);
	String * (*descriptor)(Type);

	Type (*get)(Array *, uint32_t index);
	Array * (*set)(Array *, uint32_t index, Type);
	Array * (*remove)(Array *, uint32_t index);

	Array * (*prepend)(Array *, Type);
	Array * (*append)(Array *, Type);
	Array * (*insert)(Array *, uint32_t index, Type);

	Array * (*sort)(Array *);

	String * (*description)(Array *);

	void (*dealloc)(Array *);
};

Array * ArrayAlloc(void);

#endif /* ARRAY_H */
