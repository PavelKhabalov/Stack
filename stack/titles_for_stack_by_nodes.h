#ifndef TITLES
#define TITLES
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int elem_t;

typedef struct node_s node_t;

struct node_s {
    node_t *prev_;
    elem_t value_;
};

typedef struct {
    node_t *last_;
    size_t size_;
} stack_t;

stack_t *newStack();
void push(stack_t *stack, elem_t v);
elem_t top(stack_t *stack);
int pop(stack_t *stack);
size_t sizeOfStack(stack_t *stack);
void clearStack(stack_t *stack);
void dump(stack_t *stack);

#endif
