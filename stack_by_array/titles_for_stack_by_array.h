#ifndef TITLES
#define TITLES

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <math.h>


typedef int elem_t;

enum stackErrors { NDERRORS = 0, INV_SIZE = 1, INV_CAP = 2, INV_STACK_ADRESS = 3, INV_ARR_ADRESS = 4, INV_SIZE_AND_CAP = 5, INV_ELEMS = 6 };

typedef struct {
    elem_t *array_ = NULL;
    int64_t size_ = 0;
    int64_t capacity_ = 0;
} stack_t;

stack_t newStack(int64_t capacity);

stackErrors stackOk(stack_t *stack);

int push(stack_t *stack, elem_t v);

elem_t top(stack_t *stack);

int pop(stack_t *stack);

size_t sizeOfStack(stack_t *stack);

void destroyStack(stack_t *stack);

void clearStack(stack_t *stack);

stackErrors dump(stack_t *stack);

void changeCapacity(stack_t *stack, int64_t new_capacity);

#endif
