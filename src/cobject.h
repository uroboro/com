#ifndef COBJECT_H
#define COBJECT_H

#include <stdlib.h>

#define STR(x)					#x
#define CAT(x,y)				x##y

#define SWAP(a,b)				({ typeof(a) c = a; a = b; b = c; })

#define CO_Enum(name)			typedef enum name name; enum name
#define CO_Struct(name) 		typedef struct name name; struct name

#define CO_New(T, s)			(T *)calloc((s), sizeof(T))
#define CO_Resize(v, T, s)		(T *)realloc((v), (s) * sizeof(T))
#define CO_Free(v)				free(v)

#define CO_Alloc(T) 			CAT(T,Alloc)()

#define CO_Call(o, f, ...)		({ typeof(o) obj = o; (obj && obj->f) ? obj->f(obj, ##__VA_ARGS__) : 0; })
#define CO_CallSuper(o, f, ...) ({ typeof(o) obj = o; (obj && obj->super) ? CO_Call(obj->super, f, ##__VA_ARGS__) : 0; })

#endif /* COBJECT_H */
