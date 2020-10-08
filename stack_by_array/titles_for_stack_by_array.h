#ifndef TITLES
#define TITLES

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <math.h>



enum stackErrors { NDERRORS = 0, INV_SIZE = 1, INV_CAP = 2, INV_STACK_ADRESS = 3, INV_ARR_ADRESS = 4, INV_SIZE_AND_CAP = 5, INV_ELEMS = 6, INV_OP_CANARY = 7, INV_END_CANARY = 8, INV_OP_ARR_CANARY = 9, INV_END_ARR_CANARY = 10, INV_HASH_FOR_STK = 11, INV_HASH_FOR_ARR = 12, REALLOC_ERROR = 13 };



typedef int elem_t;
typedef long long hash_t;
typedef long long canary_t;
typedef long long poison_t;

const poison_t POISON = 0xBADEDAD11ADEDA;

const long long OP_CANARY = 211884564llu;
const long long END_CANARY = 480320020llu;
const long long OP_ARR_CANARY = 748755476llu;
const long long END_ARR_CANARY = 1017190932llu;

typedef struct {
    canary_t opening_canary_ = OP_CANARY;
    elem_t *array_ = NULL;
    int64_t size_ = 0;
    int64_t capacity_ = 0;
    hash_t hash_for_stk_ = 0;
    hash_t hash_for_arr_ = 0;
    canary_t ending_canary_ = END_CANARY;
} stack_t;




void stackConstruct(stack_t *stack, int64_t capacity);

stack_t *newStack(int64_t capacity);

stackErrors stackOk(stack_t *stack);

int push(stack_t *stack, elem_t v);

elem_t top(stack_t *stack);

int pop(stack_t *stack);

size_t sizeOfStack(stack_t *stack);

void destroyStack(stack_t *stack);

void clearStack(stack_t *stack);

stackErrors dump(stack_t *stack);

stackErrors changeCapacity(stack_t *stack, int64_t new_capacity);

hash_t hFunc(void *begin, void *end);

bool checkHash(void* begin, void* end, hash_t *h);

void countHashForStack(stack_t *stack);

void setCanary(void *begin, void *end, canary_t op_canary, canary_t end_canary);

void printMem(void *begin, void *end);

void setMem(void *begin, void *end, long long filler, size_t size);


#endif
