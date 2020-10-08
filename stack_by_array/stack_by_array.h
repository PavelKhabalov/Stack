#include "titles_for_stack_by_array.h"

#define ASSERT_STACK(stack) if (int flag = stackOk(stack)) {printf("Error number: %d\n", flag); dump(stack); assert(!"OK");}
#define ARR_END(cap) ((char*) stack->array_ + (cap) * sizeof(elem_t) + 2 * sizeof(canary_t))



stack_t *newStack(int64_t capacity) {
    stack_t *stack = (stack_t*) calloc(1, sizeof(stack_t));

    stackConstruct(stack, capacity);

    return stack;
}

//-----------------------------------------------------------------------------

void stackConstruct(stack_t *stack, int64_t capacity) {
    stack->array_ = (elem_t*) calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    stack->size_ = 0;
    stack->capacity_ = capacity;

    stack->opening_canary_ = OP_CANARY;
    stack->ending_canary_ = END_CANARY;

    //setMem(stack->array_, END_ARR(stack->capacity_), POISON, sizeof(poison_t));

    setCanary(stack->array_, ARR_END(stack->capacity_), OP_ARR_CANARY, END_ARR_CANARY);

    countHashForStack(stack);

}

//-----------------------------------------------------------------------------

int push(stack_t *stack, elem_t v) {
    if (stack->size_ == 0 && stack->capacity_ == 0) {
        changeCapacity(stack, 4);
    }
    ASSERT_STACK(stack)


    if (stack->size_ == stack->capacity_) {
        changeCapacity(stack, stack->capacity_ * 2);
    }
    *(elem_t*)((char*) (stack->array_ + stack->size_++) + sizeof(canary_t)) = v;

    countHashForStack(stack);

    ASSERT_STACK(stack)
    return 0;
}

//-----------------------------------------------------------------------------

stackErrors changeCapacity(stack_t *stack, int64_t new_capacity) {
    assert(new_capacity >= 0);

    setCanary(stack->array_, ARR_END(stack->capacity_), 0, 0);

    elem_t *new_array = (elem_t*) realloc(stack->array_, new_capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    if (!new_array) {
        return REALLOC_ERROR;
    }
    stack->array_ = new_array;

    setMem(ARR_END(stack->capacity_), ARR_END(new_capacity), 0, 1);

    stack->capacity_ = new_capacity;

    setCanary(stack->array_, ARR_END(new_capacity), OP_ARR_CANARY, END_ARR_CANARY);
    countHashForStack(stack);

    ASSERT_STACK(stack)
    return NDERRORS;
}

//-----------------------------------------------------------------------------

elem_t top(stack_t *stack) {
    ASSERT_STACK(stack)
    return *(stack->array_ + stack->size_ - 1);
}

//-----------------------------------------------------------------------------

int pop(stack_t *stack) {
    ASSERT_STACK(stack)

    if (stack->size_ > 0) {
        *(elem_t*) ((char*) (stack->array_ + --stack->size_) + sizeof(canary_t)) = 0;

        if (stack->size_ * 4 < stack->capacity_ && stack->capacity_ >= 8) {
            changeCapacity(stack, stack->capacity_ / 2);
        }

        countHashForStack(stack);

        ASSERT_STACK(stack)
        return 0;
    } else {
        return 1;
    }
}

//-----------------------------------------------------------------------------

size_t sizeOfStack(stack_t *stack) {
    ASSERT_STACK(stack)
    return stack->size_;
}

//-----------------------------------------------------------------------------

stackErrors dump(stack_t *stack) {

    if (stack) {
        printf("\nAdress: %p\n\n", stack);
    } else {
        fprintf(stderr, "Error: nullptr of stack\n");
        return INV_STACK_ADRESS;
    }
    printf("OP_ARR_CANARY [%p]: %lli ||| END_ARR_CANARY [%p]: %lli\n", stack->array_, *((canary_t*) stack->array_), (canary_t*) ((char*) stack->array_ + stack->capacity_ * sizeof(elem_t) + 2 * sizeof(canary_t)) - 1, *((canary_t*) ((char*) stack->array_ + stack->capacity_ * sizeof(elem_t) + 2 * sizeof(canary_t)) - 1));
    if (stack->array_) {
        printf("Adress of buffer: %p\nSize: %lli   Capacity: %lli\n\n", stack->array_, stack->size_, stack->capacity_);
    } else {
        fprintf(stderr, "Error: nullptr of stack->array_\n");
        return INV_ARR_ADRESS;
    }

    if (stack->size_ < 0) {
        printf("Error: invalid size\n");
        return INV_SIZE;
    }

    if (stack->capacity_ < 0) {
        fprintf(stderr, "Error: invalid capacity\n");
        return INV_CAP;
    }

    if (stack->size_ > stack->capacity_) {
        fprintf(stderr, "Error: invalid size and capacity\n");
        return INV_SIZE_AND_CAP;
    }

    printMem(stack->array_, ARR_END(stack->capacity_));

    printf("Elems:\n");
    for (int i = 0; i < stack->capacity_; i++) {
        printf("%d  %p: ", i, (elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i);
        for (int j = sizeof(elem_t) - 1; j >= 0; j--) {
            printf("%02X", *((unsigned char*) ((elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i) + j));
        }
        printf("\n");

    }
    printf("\n");

    return NDERRORS;

}

//-----------------------------------------------------------------------------

stackErrors stackOk(stack_t *stack) {
    if (stack == NULL) {
        return INV_STACK_ADRESS;
    }
    if (stack->size_ < 0) {
        return INV_SIZE;
    }
    if (stack->capacity_ < 0) {
        return INV_CAP;
    }
    if (stack->size_ > stack->capacity_) {
        return INV_SIZE_AND_CAP;
    }
    if (stack->array_ == NULL) {
        return INV_ARR_ADRESS;
    }

    if (stack->opening_canary_ != OP_CANARY) {
        return INV_OP_CANARY;
    }
    if (stack->ending_canary_ != END_CANARY) {
        return INV_END_CANARY;
    }

    if (*((canary_t*) stack->array_) != OP_ARR_CANARY) {
        return INV_OP_ARR_CANARY;
    }
    if (*((canary_t*) (ARR_END(stack->capacity_)) - 1) != END_ARR_CANARY) {
        return INV_END_ARR_CANARY;
    }


    if (!checkHash(stack, stack + 1, &(stack->hash_for_stk_))) {
        return INV_HASH_FOR_STK;
    }
    if (!checkHash(stack->array_, ARR_END(stack->capacity_), &(stack->hash_for_arr_))) {
        return INV_HASH_FOR_ARR;
    }

    /*
    bool flag = false;
    for (poison_t *i = (poison_t*) ((char*) stack->array_ + sizeof(canary_t)); i < END_ARR(stack->capacity_) - sizeof(canary_t); i++) {
        flag = flag || (*i == POISON);
        if (flag && (*i != POISON) {
            return WA_EL_IN_POISON;
        }
    }
    */
    return NDERRORS;
}

//-----------------------------------------------------------------------------

void clearStack(stack_t *stack) {
    ASSERT_STACK(stack)
    for (int i = 0; i < stack->capacity_; i++) {
        stack->array_[i] = NAN;
    }
    changeCapacity(stack, 0);
    stack->size_ = 0;
    countHashForStack(stack);

}

//-----------------------------------------------------------------------------

void destroyStack(stack_t *stack) {
    ASSERT_STACK(stack)
    free(stack->array_);
    *stack = {};
}

//-----------------------------------------------------------------------------

hash_t hFunc(void *begin, void *end) {
    hash_t h = 0;

    for (size_t i = 0; (char*) begin + i < end; i++) {
        h = h * 257 + (*((unsigned char*) begin + i));
    }

    return h;
}

//-----------------------------------------------------------------------------

bool checkHash(void* begin, void* end, hash_t *h) {
    hash_t old_h = *h;
    *h = 0;

    *h = hFunc(begin, end);

    if (*h == old_h) {
        return true;
    } else {
        *h = old_h;
        return false;
    }
}

//-----------------------------------------------------------------------------

void countHashForStack(stack_t *stack) {
    stack->hash_for_arr_ = 0;
    stack->hash_for_arr_ = hFunc(stack->array_, ARR_END(stack->capacity_));

    stack->hash_for_stk_ = 0;
    stack->hash_for_stk_ = hFunc(stack, stack + 1);
}

//-----------------------------------------------------------------------------

void setCanary(void *begin, void *end, canary_t op_canary, canary_t end_canary) {
    *((canary_t*) begin) = op_canary;
    *((canary_t*) end - 1) = end_canary;
}

//-----------------------------------------------------------------------------

void printMem(void *begin, void *end) {
    printf("%p  %p\n", begin, end);
    for (unsigned char *j = (unsigned char*) end - 1; j >= begin; j--) {
        printf("%02X", *j);
    }
    printf("\n");
}

//-----------------------------------------------------------------------------

void setMem(void *begin, void *end, long long filler, size_t size) {
    long long filler_cpy = filler;
    for (size_t i = 0; (char*) begin + i < end; i++) {
        *((char*) begin + i) = *((char*) &filler + i % size);
    }
}

