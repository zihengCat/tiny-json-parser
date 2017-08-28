#ifndef _JSON_STACK_H_
#define _JSON_STACK_H_

#include <stdlib.h>     /* size_t */

typedef unsigned char byte;
typedef struct _st {
    byte  *base;
    size_t top;
    size_t size;
} st;

extern void* my_memcpy(void *dest_addr, const void *src_addr, size_t n );
extern void  init_stack(st *head);
extern void  push_stack(st *head, void *e, size_t s);
extern void* pop_stack(st *head, size_t s);
extern void  del_stack(st *head);

#endif /* json_stack_h */

