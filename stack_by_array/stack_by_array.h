#include "titles_for_stack_by_array.h"

#define ASSERT_STACK(stack) if (stackOk(stack)) {dump(stack); assert(!"OK");}




stack_t newStack(int64_t capacity) {
    stack_t stack = {};
    stack.array_ = (elem_t*) calloc(capacity, sizeof(elem_t));
    stack.size_ = 0;
    stack.capacity_ = capacity;

    for (int i = 0; i < capacity; i++) {
        stack.array_[i] = NAN;
    }

    return stack;
}

int push(stack_t *stack, elem_t v) {
    if (stack->size_ == 0 && stack->capacity_ == 0) {
        changeCapacity(stack, 4);
    }

    ASSERT_STACK(stack)

    if (stack->size_ < stack->capacity_) {
        *(stack->array_ + stack->size_++) = v;
    } else {
        changeCapacity(stack, stack->capacity_ * 2);
        *(stack->array_ + stack->size_++) = v;
    }

    ASSERT_STACK(stack)
    return 0;
}

void changeCapacity(stack_t *stack, int64_t new_capacity) {
    assert(new_capacity >= 0);

    int64_t old_cap = stack->capacity_;

    stack->array_ = (elem_t*) realloc(stack->array_, new_capacity * sizeof(elem_t));
    stack->capacity_ = new_capacity;

    for (int i = old_cap; i < stack->capacity_; i++) {
        stack->array_[i] = NAN;
    }

    ASSERT_STACK(stack)
}



elem_t top(stack_t *stack) {
    ASSERT_STACK(stack)
    return *(stack->array_ + stack->size_ - 1);
}

int pop(stack_t *stack) {
    ASSERT_STACK(stack)

    if (stack->size_ > 0) {
        stack->array_[--stack->size_] = NAN;

        if (stack->size_ * 4 < stack->capacity_ && stack->capacity_ >= 8) {
            changeCapacity(stack, stack->capacity_ / 2);
        }
        ASSERT_STACK(stack)
        return 0;
    } else {
        return 1;
    }
}

size_t sizeOfStack(stack_t *stack) {
    ASSERT_STACK(stack)
    return stack->size_;
}

stackErrors dump(stack_t *stack) {

    if (stack) {
        printf("\nAdress: %p\n\n", stack);
    } else {
        fprintf(stderr, "Error: nullptr of stack\n");
        return INV_STACK_ADRESS;
    }

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
        printf("Error: invalid capacity\n");
        return INV_CAP;
    }

    if (stack->size_ > stack->capacity_) {
        printf("Error: invalid size and capacity\n");
        return INV_SIZE_AND_CAP;
    }

    printf("Elems:\n");
    for (int i = 0; i < stack->size_; i++) {
        printf("%d  %p: ", i, stack->array_ + i);
        for (int j = sizeof(elem_t) - 1; j >= 0; j--) {
            printf("%02X", *((unsigned char*) (stack->array_ + i) + j));
        }
        printf("\n");

    }
    printf("\n");
    return NDERRORS;

}

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

    bool flag = false;
    for (int i = 0; i < stack->capacity_; i++) {
        flag = (stack->array_[i] != stack->array_[i]);
        if (flag && !(stack->array_[i] != stack->array_[i])) {
            return INV_ELEMS;
        }
    }


    return NDERRORS;
}

void clearStack(stack_t *stack) {
    ASSERT_STACK(stack)
    for (int i = 0; i < stack->capacity_; i++) {
        stack->array_[i] = NAN;
    }
    changeCapacity(stack, 0);
    stack->size_ = 0;

}


void destroyStack(stack_t *stack) {
    ASSERT_STACK(stack)
    free(stack->array_);
    *stack = {};
}





