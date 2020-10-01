#include "titles_for_stack_by_array.h"


stack_t newStack() {
    stack_t stack = {};
    stack.array_ = (elem_t*) calloc(1, sizeof(elem_t));
    stack.size_ = 0;
    stack.capacity_ = 10;

    return stack;
}

int push(stack_t *stack, elem_t v) {
    if (stack->size_ < stack->capacity_) {
        *(stack->array_ + stack->size_++) = v;
    } else {
        stack->array_ = (elem_t*) realloc(stack->array_, stack->capacity_ * 2);
        stack->capacity_ *= 2;
        *(stack->array_ + stack->size_++) = v;
    }
    return 0;
}

elem_t top(stack_t *stack) {
    return *(stack->array_ + stack->size_ - 1);
}

int pop(stack_t *stack) {
    if (stack->size_ > 0) {
        stack->size_--;
        if (stack->size_ * 4 < stack->capacity_) {
            stack->array_ = (elem_t*) realloc(stack->array_, stack->capacity_ / 2);
            stack->capacity_ /= 2;
        }
        return 0;
    } else {
        return 1;
    }
}

size_t sizeOfStack(stack_t *stack) {
    return stack->size_;
}

void clearStack(stack_t *stack) {
    free(stack->array_);
    *stack = {};
}





