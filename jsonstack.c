#include "jsonstack.h"
#include <stdlib.h>     /* malloc, realloc */

extern void* my_memcpy(void *dest_addr, const void *src_addr, size_t n ) {
     char* dest = dest_addr;
     const char* src  = src_addr;
     while( n > 0 ) {
        *dest++ = *src++;
        n--;
     }
    return dest_addr;
}

#define STACK_INIT_NUM 1024
extern void init_stack(st *head) {
    head->base  = (byte*)malloc(sizeof(byte) * STACK_INIT_NUM);
    head->top   = 0;
    head->size  = sizeof(byte) * STACK_INIT_NUM;
}
#undef STACK_INIT_NUM

extern void push_stack(st *head, void *e, size_t s) {
    while((head->size - head->top) < s) {
        head->base = (byte*)realloc(head->base, head->size * 2);
        head->size *= 2;
    }
    my_memcpy(head->base + head->top, e, s);
    head->top += s;
}
extern void* pop_stack(st *head, size_t s) {
    if(head->top == 0) {
        return NULL;
    }
    head->top -= s;
    return (void*)(head->base + head->top);
}

void del_stack(st *head) {
    if(head->top != 0) {
    }
    free(head->base);
    head->size = 0;
    head->top  = 0;
}

#if 0
#define TESTNUM 1024
int main(void) {

    st s;
    int i;
    init_stack(&s);
    void* p = malloc(TESTNUM*2);
    push_stack(&s, p, TESTNUM*2);
    printf("%p\n", pop_stack(&s, TESTNUM*2));
    del_stack(&s);

#if 0
    for(i = 0; i<100; i++) {
        double d = i;
        push_stack(&s, &d, sizeof(d));
    }
    for(i = 0; i<100; i++) {
        printf("%.2lf ", *((double*)(pop_stack(&s, sizeof(double)))));
    }
    printf("\n");
#endif
    return 0;
}
#endif

