#include <stdio.h>
#include <string.h>

#include "../src/dictionary.h"

void *strIdentity(void *p) { return p; }

int main(int argc, char **argv, char **envp) {
	Dictionary *d = CO_Alloc(Dictionary);
	d->retainer = (ContainerRetainer)strdup;
	d->releaser = (ContainerReleaser)free;
	//d->descriptor = (ContainerDescriptor)strIdentity;

	CO_Call(d, set, "foo", "hello world");
	CO_Call(d, set, "foo2", "hello world2");
	CO_Call(d, description);

	printf("set foo text\n");
	CO_Call(d, set, "foo", "hello world2");
	printf("dict[\"foo\"]? %s\n", CO_Call(d, get, "foo"));

	printf("rem foo\n");
	CO_Call(d, set, "foo", NULL);
	CO_Call(d, description);

	printf("rem foo\n");
	CO_Call(d, set, "foo", NULL);
	printf("dict[\"foo\"]? %s\n", CO_Call(d, get, "foo"));

	printf("set bar\n");
	char v[] = { 64, 32, 87, 0, };
	CO_Call(d, set, "bar", v);
	CO_Call(d, description);

	char buf[512];
	for (int i = 0; i < 100; i++) {
		sprintf(buf, "%d", i);
		CO_Call(d, set, buf, buf);
	}
	CO_Call(d, description);

	CO_Call(d, dealloc);

	return 0;
}
