#include <string.h>
#include <stdio.h>

#include "array.h"

#define INITIAL_SIZE (10)
#define GROWTH_FACTOR (2)

static Type ArrayGet(Array * a, uint32_t index);
static Array * ArraySet(Array * a, uint32_t index, Type value);
static Array * ArrayRemove(Array * a, uint32_t index);

static Array * ArrayPushFroward(Array * a, uint32_t index);
static Array * ArrayPushBack(Array * a, uint32_t index);

static Array * ArrayPrepend(Array * a, Type value);
static Array * ArrayAppend(Array * a, Type value);
static Array * ArrayInsert(Array * a, uint32_t index, Type value);

static Array * ArraySort(Array * a);

static String * ArrayDescription(Array * a);

static void ArrayDealloc(Array * a);

static Array * ArrayAllocInternal(int size);
static Array * ArrayGrow(Array * a);
Array * ArrayAlloc(void);


static Type ArrayGet(Array * a, uint32_t index) {
	if (a) {
		if (index < a->n) {
			void ** table = (void **)a->data;
			return table[index];
		}
	}
	return NULL;
}

static Array * ArrayInsertRaw(Array * a, uint32_t index, Type value) {
	if (a) {
		void ** table = (void **)a->data;
		table[index] = a->retainer ? a->retainer(value) : value;
	}
	return a;
}
static Array * ArraySet(Array * a, uint32_t index, Type value) {
	if (a) {
		if (index < a->size && value) {
			void ** table = (void **)a->data;
			if (a->releaser) {
				a->releaser(table[index]);
			}
			ArrayInsertRaw(a, index, value);
			// table[index] = a->retainer ? a->retainer(value) : value;
		}
	}
	return a;
}

static Array * ArrayRemove(Array * a, uint32_t index) {
	if (a) {
		Type value = ArrayGet(a, index);
		if (value) {
			if (a->releaser) {
				a->releaser(value);
			}
			void ** table = (void **)a->data;
			table[index] = NULL;
			a->n--;
			while (table[index+1]) {
				SWAP(table[index], table[index+1]);
				index++;
			}
			if (0) ArrayPushFroward(a, 0);
		}
	}
	return a;
}

static Array * ArrayPushFroward(Array * a, uint32_t index) {
	if (a) {
		if (index >= a->n) {
			return a;
		}

		void ** table = (void **)a->data;
		for (uint32_t idx = index; index < a->n - 1; index++) {
			if (!table[idx]) {
				SWAP(table[idx], table[idx+1]);
			}
		}
	}
	return a;
}
static Array * ArrayPushBack(Array * a, uint32_t index) {
	if (a) {
		if (index >= a->n) {
			return a;
		}

		void ** table = (void **)a->data;
		for (uint32_t idx = a->n - 1; idx > index; idx--) {
			if (!table[idx]) {
				SWAP(table[idx], table[idx-1]);
			}
		}
	}
	return a;
}

static Array * ArrayPrepend(Array * a, Type value) {
	if (a) {
		if (value) {
			if (a->n == a->size) {
				ArrayGrow(a);
			}
			ArrayInsert(a, 0, value);
		}
	}
	return a;
}
static Array * ArrayAppend(Array * a, Type value) {
	if (a) {
		if (value) {
			if (a->n == a->size) {
				ArrayGrow(a);
			}
			ArrayInsert(a, a->n, value);
		}
	}
	return a;
}

static Array * ArrayInsert(Array * a, uint32_t index, Type value) {
	if (a) {
		if (index < a->size && value) {
			ArrayPushBack(a, index);
			printf("<%p>(%d) add <%s>\n", a, a->n, a->descriptor ? a->descriptor(value) : value);
			ArraySet(a, index, value);
			a->n++;
		}
	}
	return a;
}

static Array * ArraySort(Array * a) {
	if (a) {
		//quick sort
		//lol right
	}
	return a;
}

static String * ArrayDescription(Array * a) {
	if (a) {
		void ** table = (void **)a->data;
		printf("%d values {\n", a->n);
		for (ssize_t i = 0; i < a->size; i++) {
			void * value = table[i];
			if (value) {
				printf("\t%s\n", a->descriptor ? a->descriptor(value) : value);
			}
		}
		printf("}\n");
	}
	return NULL;
}

static void ArrayDealloc(Array * a) {
	if (a) {
		if (a->data) {
			void ** table = (void **)a->data;
			for (size_t i = 0; i < a->size; i++) {
				if (table[i] && a->releaser) {
					a->releaser(table[i]);
				}
			}
		}

		CO_Free(a->data);
		CO_Free(a);
	}
	return;
}

/* dictionary initialization code used in both DictionaryAlloc and grow */
static Array * ArrayAllocInternal(int size) {
	Array * a = CO_New(Array, 1);
	if (a) {
		a->n = 0;
		a->size = size;
		a->data = CO_New(void * , a->size);
		if (a->data == NULL) {
			CO_Free(a);
			return NULL;
		}
	}
	return a;
}


static Array * ArrayGrow(Array * a) {
	if (a) {
		Array * a2 = ArrayAllocInternal(a->size * GROWTH_FACTOR);
		if (a2) {
			void ** table = (void **)a->data;
			void ** table2 = (void **)a2->data;
			for (size_t i = 0; i < a->size; i++) {
				void * value = table[i];
				if (value) {
					table2[i] = table[i];
				}
			}

			SWAP(a->data, a2->data);
			SWAP(a->size, a2->size);

			ArrayDealloc(a2);
		}
	}
	return a;
}

Array * ArrayAlloc(void) {
	Array * a = ArrayAllocInternal(INITIAL_SIZE);
	if (a) {
		a->get = ArrayGet;
		a->set = ArraySet;
		a->remove = ArrayRemove;
		a->prepend = ArrayPrepend;
		a->append = ArrayAppend;
		a->insert = ArrayInsert;
		a->sort = ArraySort;
		a->description = ArrayDescription;
		a->dealloc = ArrayDealloc;
	}
	return a;
}
