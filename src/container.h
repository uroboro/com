#ifndef CONTAINER_H
#define CONTAINER_H

#include "cobject.h"

typedef void * Type;
typedef char String;

typedef Type (*ContainerRetainer)(Type);
typedef void (*ContainerReleaser)(Type);
typedef String *(*ContainerDescriptor)(Type);

CO_Enum(ContainerComparisonResult) {
CCRDescending = -1,
CCREqual = 0,
CCRAscending = +1,
};

typedef int (*ContainerComparator)(Type, Type);

#endif /* CONTAINER_H */
