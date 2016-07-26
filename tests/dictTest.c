#include <stdio.h>
#include <string.h>

#include "../src/dictionary.h"

int main(int argc, char **argv, char **envp) {
	Dictionary *object = CO_Alloc(Dictionary);
	object->retainer = (ContainerRetainer)strdup;
	object->releaser = (ContainerReleaser)free;
	//d->descriptor = (ContainerDescriptor)NULL;

	CO_Call(object, set, "foo", "hello world");
	CO_Call(object, set, "foo2", "hello world2");
	CO_Call(object, description);

	printf("set foo text\n");
	CO_Call(object, set, "foo", "hello world2");
	printf("dict[\"foo\"]? %s\n", CO_Call(object, get, "foo"));

	printf("rem foo\n");
	CO_Call(object, set, "foo", NULL);
	CO_Call(object, description);

	printf("rem foo\n");
	CO_Call(object, set, "foo", NULL);
	printf("dict[\"foo\"]? %s\n", CO_Call(object, get, "foo"));

	printf("set bar\n");
	char v[] = { 64, 32, 87, 0, };
	CO_Call(object, set, "bar", v);
	CO_Call(object, description);

	char buf[512];
	for (int i = 0; i < 100; i++) {
		sprintf(buf, "%d", i);
		CO_Call(object, set, buf, buf);
	}
	CO_Call(object, description);

	CO_Call(object, dealloc);

	return 0;
}
