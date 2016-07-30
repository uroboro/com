#include <stdio.h>
#include <string.h>

#include "../src/array.h"

int main(int argc, char **argv, char **envp) {
	Array *object = CO_Alloc(Array);
	object->retainer = (ContainerRetainer)strdup;
	object->releaser = (ContainerReleaser)free;
	//d->descriptor = (ContainerDescriptor)NULL;

	CO_Call(object, append, "hello world");
	CO_Call(object, append, "hello world2");
	CO_Call(object, append, "hello world3");
	CO_Call(object, description);

	printf("set [1]\n");
	CO_Call(object, set, 1, "hello 3");
	printf("array[1]? %s\n", CO_Call(object, get, 1));

	printf("rem [1]\n");
	CO_Call(object, remove, 1);
	printf("array[1]? %s\n", CO_Call(object, get, 1));

	printf("set bar\n");
	char v[] = { 64, 32, 87, 0, };
	CO_Call(object, append, v);
	CO_Call(object, description);

	char buf[512];
	for (int i = 0; i < 10; i++) {
		sprintf(buf, "%d", i);
		if (i%2) {
			CO_Call(object, append, buf);
		} else {
			CO_Call(object, prepend, buf);
		}
	}
	CO_Call(object, description);

	CO_Call(object, dealloc);

	return 0;
}
