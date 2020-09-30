#include <stdio.h>


struct node {
    node* prev_;
    int value_;
};

struct my_stack {
    node* last_;
    size_t size_;
};

void push(struct my_stack* stack, int v) {
    struct node* new_node = (node*) calloc(1, sizeof(node);
    new_node->prev_ = stack->last_;
    new_node->value_ = v;
    stack->size_++;
    stack->last_ = new_node;
}

int top(struct my_stack stack) {
    return stack.last_->value_;
}

int pop(struct my_stack* stack) {
    if (stack->size_ != 0) {
        node* last = stack->last_;
        stack->last_ = stack->last_->prev;
        free(last);
        return 0;
    }
    else {
        return -1;
    }
}



