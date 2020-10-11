#include "titles_for_stack_by_array.h"

#define ASSERT_STACK(stack) if (int flag = stackOk(stack)) {printf("Error status: %d\nFile: '%s'\nLine: %d, in '%s' function\n-------------\n", flag, __FILE__, __LINE__, __func__); stackDump(stack, flag); assert(!"OK");}
#define ARR_END(cap) ((char*) stack->array_ + (cap) * sizeof(elem_t) + 2 * sizeof(canary_t))



stack_t *newStack(int64_t capacity) {
    stack_t *stack = (stack_t*) calloc(1, sizeof(stack_t));

    stackConstruct(stack, capacity);

    return stack;
}

//-----------------------------------------------------------------------------

stack_t *stackConstruct(stack_t *stack, int64_t capacity) {
    stack->array_ = (elem_t*) calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    stack->size_ = 0;
    stack->capacity_ = capacity;

    #if DEF_LVL > 0
    stack->opening_canary_ = OP_CANARY;
    stack->ending_canary_ = END_CANARY;

    setCanary(stack->array_, ARR_END(stack->capacity_), OP_ARR_CANARY, END_ARR_CANARY);
    #endif

    #if DEF_LVL > 1
    countHashForStack(stack);
    #endif
    //setMem(stack->array_, END_ARR(stack->capacity_), POISON, sizeof(poison_t));

    return stack;

}

//-----------------------------------------------------------------------------

int stackPush(stack_t *stack, elem_t v) {
    ASSERT_STACK(stack)


    if (stack->size_ == stack->capacity_) {
        changeStackCapacity(stack, max(stack->capacity_ * 2, 4));
    }
    *(elem_t*)((char*) (stack->array_ + stack->size_++) + sizeof(canary_t)) = v;

    #if DEF_LVL > 1
    countHashForStack(stack);
    #endif
    ASSERT_STACK(stack)
    return 0;
}

//-----------------------------------------------------------------------------

stackErrors changeStackCapacity(stack_t *stack, int64_t new_capacity) {
    assert(new_capacity >= 0);
    #if DEF_LVL > 0
    setCanary(stack->array_, ARR_END(stack->capacity_), 0, 0);
    #endif
    elem_t *new_array = (elem_t*) realloc(stack->array_, new_capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    if (!new_array) {
        return REALLOC_ERROR;
    }
    stack->array_ = new_array;

    setMem(ARR_END(stack->capacity_), ARR_END(new_capacity), 0, 1);

    stack->capacity_ = new_capacity;

    #if DEF_LVL > 0
    setCanary(stack->array_, ARR_END(new_capacity), OP_ARR_CANARY, END_ARR_CANARY);
    #endif

    #if DEF_LVL > 1
    countHashForStack(stack);
    #endif

    ASSERT_STACK(stack)
    return NDERRORS;
}

//-----------------------------------------------------------------------------

elem_t stackTop(stack_t *stack) {
    ASSERT_STACK(stack)
    return *(stack->array_ + stack->size_ - 1);
}

//-----------------------------------------------------------------------------

int stackPop(stack_t *stack) {
    ASSERT_STACK(stack)

    if (stack->size_ > 0) {
        *(elem_t*) ((char*) (stack->array_ + --stack->size_) + sizeof(canary_t)) = 0;

        if (stack->size_ * 4 < stack->capacity_ && stack->capacity_ >= 8) {
            changeStackCapacity(stack, stack->capacity_ / 2);
        }
        #if DEF_LVL > 1
        countHashForStack(stack);
        #endif
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

stackErrors stackDump(stack_t *stack, long long status) {

    printf("\n---------begin-of-dump--------\n");
#if STK_DEBUG == 1
    if (stack) {
        printf("\nAdress: %p\n\n", stack);
    } else {
        fprintf(stderr, "Error: nullptr of stack\n");
        return INV_STACK_ADRESS;
    }

    #if DEF_LVL > 1
    if (status & INV_HASH_FOR_STK) {
        hash_t old_h = stack->hash_for_stk_;
        stack->hash_for_stk_ = 0;
        printf("Error: incorrect hash for stack: %llu, must be %llu\n", hFunc(stack, stack + 1), old_h);
        stack->hash_for_stk_ = old_h;
    } else {
        printf("Hash for stack: %llu\n", stack->hash_for_stk_);
    }
    if (status & INV_HASH_FOR_ARR) {
        hash_t old_h = stack->hash_for_arr_;
        stack->hash_for_arr_ = 0;
        printf("Error: incorrect hash for array: %llu, must be %llu\n", hFunc(stack->array_, ARR_END(stack->capacity_)), old_h);
        stack->hash_for_arr_ = old_h;
    } else {
        printf("Hash for array: %llu\n", stack->hash_for_arr_);
    }
    printf("\n");
    #endif

    #if DEF_LVL > 0
    if (status & INV_OP_CANARY) {
        printf("Error: incorrect opening canary of stack: %llu, must be %llu\n", stack->opening_canary_, OP_CANARY);
    } else {
        printf("Opening stack canary: %llu\n", stack->opening_canary_);
    }
    if (status & INV_END_CANARY) {
        printf("Error: incorrect ending canary of stack: %llu, must be %llu\n", stack->ending_canary_, END_CANARY);
    } else {
        printf("Opening stack canary: %llu\n", stack->ending_canary_);
    }
    printf("\n");
    #endif

    if (stack->array_) {
        printf("Adress of array: %p\nSize: %lli   Capacity: %lli\n", stack->array_, stack->size_, stack->capacity_);
    } else {
        fprintf(stderr, "Error: nullptr of stack->array_\n");
        return INV_ARR_ADRESS;
    }

    if (status & INV_SIZE) {
        printf("Error: invalid size: %lld\n", stack->size_);
    }

    if (status & INV_CAP) {
        fprintf(stderr, "Error: invalid capacity: %lld\n", stack->capacity_);
    }

    if (status & INV_SIZE_AND_CAP) {
        fprintf(stderr, "Error: invalid size and capacity: %lld > %lld\n", stack->size_, stack->capacity_);
    }
    printf("\n");
    #if DEF_LVL > 0
    if (status & INV_OP_ARR_CANARY) {
        printf("Error: incorrect opening canary of array: %llu, must be %llu\n", *((canary_t*) stack->array_), OP_ARR_CANARY);
    } else {
        printf("Opening arr canary: %llu\n", *((canary_t*) stack->array_));
    }
    if (status & INV_END_ARR_CANARY) {
        printf("Error: incorrect ending canary of array: %llu, must be %llu\n", *((canary_t*) (ARR_END(stack->capacity_)) - 1), END_ARR_CANARY);
    } else {
        printf("Opening arr canary: %llu\n", *((canary_t*) stack->array_));
    }
    #endif
    printf("\nData of array: ");
    printMem(stack->array_, ARR_END(stack->capacity_));
#else
    if (status & INV_STACK_ADRESS) {
        printf("Error: invalid adress of stack: %p\n", stack);
        return INV_STACK_ADRESS;
    }
    if (status & INV_OP_CANARY) {
        printf("Error: incorrect opening canary for stack: %llu\n", stack->opening_canary_);
    }
    if (status & INV_END_CANARY) {
        printf("Error: incorrect ending canary for stack: %llu\n", stack->ending_canary_);
    }
    if (status & INV_ARR_ADRESS) {
        printf("Error: invalid adress of array of stack: %p\n", stack->array_);
    }
    if (status & INV_SIZE) {
        printf("Error: invalid size: %lld\n", stack->size_);
    }
    if (status & INV_CAP) {
        printf("Error: invalid capacity: %lld\n", stack->capacity_);
    }
    if (status & INV_SIZE_AND_CAP) {
        printf("Error: invalid size and capacity: %lld > %lld\n", stack->size_, stack->capacity_);
    }
    if (status & INV_OP_ARR_CANARY) {
        printf("Error: incorrect opening canary for array: %llu\n", *(canary_t*) stack->array_);
    }
    if (status & INV_END_ARR_CANARY) {
        printf("Error: incorrect ending canary for array: %llu\n", *(canary_t*) (ARR_END(stack->capacity_)));
    }
    if (status & INV_HASH_FOR_STK) {
        hash_t old_h = stack->hash_for_stk_;
        stack->hash_for_stk_ = 0;
        printf("Error: incorrect hash for stack: %llu, must be %llu\n", hFunc(stack, stack + 1), old_h);
        stack->hash_for_stk_ = old_h;
    }
    if (status & INV_HASH_FOR_ARR) {
        hash_t old_h = stack->hash_for_arr_;
        stack->hash_for_arr_ = 0;
        printf("Error: incorrect hash for array: %llu, must be %llu\n", hFunc(stack->array_, ARR_END(stack->capacity_)), old_h);
        stack->hash_for_arr_ = old_h;
    }

#endif
    printf("Elems:\n");
    for (int i = 0; i < stack->capacity_; i++) {
        printf("%d  %p: ", i, (elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i);
        for (int j = sizeof(elem_t) - 1; j >= 0; j--) {
            printf("%02X", *((unsigned char*) ((elem_t*) ((char*) stack->array_ + sizeof(canary_t)) + i) + j));
        }
        printf("\n");

    }
    printf("\n---------end-of-dump--------\n");

    return NDERRORS;

}

//-----------------------------------------------------------------------------

long long stackOk(stack_t *stack) {

    long long status = 0;

    if (stack == NULL) {
        status = status | INV_STACK_ADRESS;
        return status;
    }

    if (stack->size_ < 0) {
        status = status | INV_SIZE;
    }
    if (stack->capacity_ < 0) {
        status = status | INV_CAP;
    }
    if (stack->size_ > stack->capacity_) {
        status = status | INV_SIZE_AND_CAP;
    }
    #if DEF_LVL > 0
    if (stack->opening_canary_ != OP_CANARY) {
        status = status | INV_OP_CANARY;
    }
    if (stack->ending_canary_ != END_CANARY) {
        status = status | INV_END_CANARY;
    }
    #endif

    #if DEF_LVL > 1
    if (!checkHash(stack, stack + 1, &(stack->hash_for_stk_))) {
        status = status | INV_HASH_FOR_STK;
    }
    #endif

    if (stack->array_ == NULL) {
        status = status | INV_ARR_ADRESS;
        return status;
    }
    #if DEF_LVL > 0
    if (*((canary_t*) stack->array_) != OP_ARR_CANARY) {
        status = status | INV_OP_ARR_CANARY;
    }
    if (*((canary_t*) (ARR_END(stack->capacity_)) - 1) != END_ARR_CANARY) {
        status = status | INV_END_ARR_CANARY;
    }
    #endif

    #if DEF_LVL > 1
    if (!checkHash(stack->array_, ARR_END(stack->capacity_), &(stack->hash_for_arr_))) {
        status = status | INV_HASH_FOR_ARR;
    }
    #endif
    /*
    bool flag = false;
    for (poison_t *i = (poison_t*) ((char*) stack->array_ + sizeof(canary_t)); i < END_ARR(stack->capacity_) - sizeof(canary_t); i++) {
        flag = flag || (*i == POISON);
        if (flag && (*i != POISON) {
            return WA_EL_IN_POISON;
        }
    }
    */
    return status;
}

//-----------------------------------------------------------------------------

void clearStack(stack_t *stack) {
    ASSERT_STACK(stack)
    for (int i = 0; i < stack->capacity_; i++) {
        stack->array_[i] = NAN;
    }
    changeStackCapacity(stack, 0);
    stack->size_ = 0;
    #if DEF_LVL > 1
    countHashForStack(stack);
    #endif

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

