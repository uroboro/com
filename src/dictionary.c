#include <string.h>
#include <stdio.h>

#include "cobject.h"
#include "dictionary.h"

static unsigned long hash_function(const char *str) {
	unsigned long hash = 0;
	#if 0
	#define MULTIPLIER	97
	for (unsigned const char *us = (unsigned const char *) str; *us; us++) {
		hash = hash * MULTIPLIER + *us;
	}
	#else
	// sdbm
	unsigned char c;
	while ((c = *str++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	#endif
	return hash;
}

CO_Struct(KVPair) {
	char *key;
	Type value;
};

static KVPair *KVPairNew(const char *key, Type value, Type (*retainer)(Type t)) {
	if (key == NULL || key[0] == 0 || value == NULL) {
		return NULL;
	}

	KVPair *e = CO_New(KVPair, 1);
	if (e) {
		e->key = strdup(key);
		e->value = retainer ? retainer(value) : value;
	}

	return e;
}

static void KVPairFree(KVPair *e, void (*releaser)(Type t)) {
	if (e) {
		free(e->key);
		if (releaser) {
			releaser(e->value);
		}
		CO_Free(e);
	}
}

static KVPair *KVPairCopy(KVPair *e, Type (*retainer)(Type t)) {
	return KVPairNew(e->key, e->value, retainer);

}

#define INITIAL_SIZE (60)
#define GROWTH_FACTOR (2)

static KVPair *DictionaryGetInternal(Dictionary *d, const char *key);
static Type DictionaryGet(Dictionary *d, const char *key);

static Dictionary *DictionarySet(Dictionary *d, const char *key, Type value);
static Dictionary *DictionaryUnset(Dictionary *d, const char *key);
static Dictionary *DictionaryInsertPair(Dictionary *d, KVPair *e);
static Dictionary *DictionaryAddPair(Dictionary *d, KVPair *e);

static String *DictionaryDescription(Dictionary *d);

static void DictionaryDelete(Dictionary *d);

static Dictionary *DictionaryAllocInternal(int size);
static Dictionary *DictionaryGrowTable(Dictionary *d);

Dictionary *DictionaryAlloc(void);


static KVPair *DictionaryGetInternal(Dictionary *d, const char *key) {
	if (key == NULL || key[0] == 0) {
		return NULL;
	}
	KVPair **table = (KVPair **)d->table;
	return table[hash_function(key) % d->size];
}

static Type DictionaryGet(Dictionary *d, const char *key) {
	KVPair *e = DictionaryGetInternal(d, key);
	if (e) {
		if (!strcmp(e->key, key)) {
			return e->value;
		}
	}

	return NULL;
}

static Dictionary *DictionarySet(Dictionary *d, const char *key, Type value) {
	if (key == NULL || key[0] == 0) {
		return d;
	}

	if (value == NULL) {
		DictionaryUnset(d, key);
	} else {
		DictionaryAddPair(d, KVPairNew(key, value, d->retainer));
	}

	return d;
}

static Dictionary *DictionaryUnset(Dictionary *d, const char *key) {
	KVPair *e = DictionaryGetInternal(d, key);
	if (e) {
		KVPairFree(e, d->releaser);
		d->n--;
		KVPair **table = (KVPair **)d->table;
		table[hash_function(key) % d->size] = NULL;
	}

	return d;
}

static Dictionary *DictionaryInsertPair(Dictionary *d, KVPair *e) {
	if (e) {
		KVPair **table = (KVPair **)d->table;
		table[hash_function(e->key) % d->size] = e;
		d->n++;
	}

	return d;
}

static Dictionary *DictionaryAddPair(Dictionary *d, KVPair *e) {
	if (e == NULL || e->value == NULL) {
		return NULL;
	}

	KVPair *ep = DictionaryGetInternal(d, e->key);
	if (ep) { // Existing value for hash(key)
		if (!strcmp(ep->key, e->key)) { // Keys are equal, replace
			KVPairFree(ep, d->releaser);
			d->n--;
		} else { // Keys are different, grow dictionary
			DictionaryGrowTable(d);
		}
	}
	DictionaryInsertPair(d, e);

	return d;
}

static String *DictionaryDescription(Dictionary *d) {
	if (d) {
		KVPair **table = (KVPair **)d->table;
		printf("%d pairs {\n", d->n);
		for (ssize_t i = 0; i < d->size; i++) {
			KVPair *e = table[i];
			if (e) {
				printf("\t\"%s\" : %s\n", e->key, d->descriptor ? d->descriptor(e->value) : e->value);
			}
		}
		printf("}\n");
	}

	return NULL;
}

/* dictionary initialization code used in both DictionaryAlloc and grow */
static Dictionary *DictionaryAllocInternal(int size) {
	Dictionary *d = CO_New(Dictionary, 1);
	if (d) {
		d->n = 0;
		d->size = size;
		d->table = CO_New(KVPair *, d->size);
		if (d->table == NULL) {
			CO_Free(d);
			return NULL;
		}
	}

	return d;
}

static Dictionary *DictionaryGrowTable(Dictionary *d) {
	if (d) {
		Dictionary *d2 = DictionaryAllocInternal(d->size * GROWTH_FACTOR);
		if (d2 != NULL) {
			KVPair **table = (KVPair **)d->table;
			for (size_t i = 0; i < d->size; i++) {
				KVPair *e = table[i];
				if (e) {
					DictionaryInsertPair(d2, KVPairCopy(e, d->retainer));
				}
			}

			SWAP(d->table, d2->table);
			SWAP(d->size, d2->size);

			DictionaryDelete(d2);
		}
	}
	return d;
}

void DictionaryDelete(Dictionary *d) {
	if (d) {
		if (d->table) {
			KVPair **table = (KVPair **)d->table;
			for (size_t i = 0; i < d->size; i++) {
				KVPairFree(table[i], d->releaser);
			}
		}

		CO_Free(d->table);
		CO_Free(d);
	}
}

Dictionary *DictionaryAlloc(void) {
	Dictionary *d = DictionaryAllocInternal(INITIAL_SIZE);
	if (d) {
		d->get = DictionaryGet;
		d->set = DictionarySet;
		d->description = DictionaryDescription;
		d->dealloc = DictionaryDelete;
	}
	return d;
}
