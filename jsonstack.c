#ifndef _JSON_STACK_H_
#define _JSON_STACK_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "leptjson.h"

typedef int type;

typedef struct _st {
    type  *base;
    size_t top;
    size_t size;
} st;

void init_stack(st *head) {
    head->base  = (type*)malloc(sizeof(type)*10);
    head->top   = 0;
    head->size  = sizeof(type)*10;
}

void push_stack(st *head, type *e) {
    memcpy((unsigned char*)head->base + head->top, e, sizeof(type));
    head->top += sizeof(type);
}
type* pop_stack(st *head) {
    if(head->top == 0) {
        return NULL;
    }
    head->top -= sizeof(type);
    return (type*)((unsigned char*)head->base + head->top);
}

#endif /* json_stack_h */

int main(void) {
    st s;
    int i;
    init_stack(&s);
    for(i = 0; i<10; i++) {
        push_stack(&s, &i);
    }
    for(i = 0; i<10; i++) {
        printf("%d ", *(pop_stack(&s)));
    }
    printf("\n");

    return 0;
}
