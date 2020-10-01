#include "titles_for_stack_by_nodes.h"

#define ASSERT_STACK(stack) //if (stack == NULL) {fprintf(stderr, "Error: stack hadh't been initialized\n"); assert(0);} else if (stack->last_ == NULL) {fprintf(stderr, "Error: invalid adress of last element: NULL. It may be in two cases: stack hadn't been initialized or for last element was not some member"); assert(0);}


stack_t *newStack() {
    stack_t* stk = (stack_t*) calloc(1, sizeof(stack_t));
    *stk = {0, 0};
    return stk;
}

void push(stack_t *stack, elem_t v) {
    node_t *new_node = (node_t*) calloc(1, sizeof(node_t));
    assert(new_node);

    new_node->prev_ = stack->last_;
    new_node->value_ = v;

    stack->size_++;
    stack->last_ = new_node;

    ASSERT_STACK(stack)
}

elem_t top(stack_t *stack) {
    ASSERT_STACK(stack)

    return stack->last_->value_;
}

int pop(stack_t *stack) {
    ASSERT_STACK(stack)
    assert(stack);

    if (stack->size_ > 0) {
        //dump(stack);

        node_t *last = stack->last_;
        stack->last_ = stack->last_->prev_;
        free(last);
        stack->size_--;

        ASSERT_STACK(stack)
        return 0;
    } else {
        return -1;
    }
}

size_t sizeOfStack(stack_t *stack) {
    ASSERT_STACK(stack)

    return stack->size_;
}

void clearStack(stack_t *stack) {
    ASSERT_STACK(stack)

    while (stack->size_ > 0) {
        pop(stack);
    }
}

void dump(stack_t *stack) {
    node_t *l = stack->last_;
    printf("> %p %zu\n", stack, stack->size_);

    for (int i = stack->size_; i > 1; i--) {
        printf("%p %d\n", l, l->value_);
        printf("%p\n\n\n", l->prev_);

        l = l->prev_;
    }
    printf("%p %d\n", l, l->value_);
}






