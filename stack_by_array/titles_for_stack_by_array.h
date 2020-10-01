#ifndef TITLES
#define TITLES

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int elem_t;

typedef struct {
    elem_t *array_ = NULL;
    size_t size_ = 0;
    size_t capacity_ = 0;
} stack_t;

stack_t newStack();

int push(stack_t *stack, elem_t v);

elem_t top(stack_t *stack);

int pop(stack_t *stack);

size_t sizeOfStack(stack_t *stack);

void clearStack(stack_t *stack);

#endif
