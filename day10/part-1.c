#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stack_node {
    char value;
    struct stack_node * next;
};

struct stack {
    size_t length;
    struct stack_node * top;
};

struct stack * create_stack() {
    struct stack * stack = (struct stack *) malloc(sizeof(struct stack));

    stack->top = NULL;
    stack->length = 0;
    
    return stack;
};

void stack_push(struct stack * stack, char value) {
    struct stack_node * old_top = stack->top;
    struct stack_node * new_top = (struct stack_node *) malloc(sizeof(struct stack_node));

    new_top->next = old_top;
    new_top->value = value;
    stack->top = new_top;
    stack->length++;
}

struct stack_node * stack_pop(struct stack * stack) {
    struct stack_node * result = NULL;

    if (stack->length) {
        stack->length--;
        result = stack->top;
        stack->top = stack->top->next;
        result->next = NULL;
    }

    return result;
}

void free_stack_node(struct stack_node * node) {
    if (node == NULL) return;

    free_stack_node(node->next);
    free(node);
}

void print_stack(struct stack * stack) {
    struct stack_node * node = stack->top;

    printf("stack (%2u items): ", stack->length);

    while (node != NULL) {
        printf("%c -> ", node->value);
        node = node->next;
    }
    puts("NULL");
}

int main() {
    int c;
    struct stack * stack = create_stack();
    unsigned int error_score = 0;

    assert(stack->length == 0);
    assert(stack->top == NULL);

    while ((c = getc(stdin)) != EOF) {
        print_stack(stack);

        switch (c) {
            case '\n': {
                puts("found \\n");
                free_stack_node(stack->top);
                stack->top = NULL;
                stack->length = 0;
                break;
            }
            case '(':
            case '[':
            case '{':
            case '<': {
                stack_push(stack, (char) c);
                break;
            }
            case ')': {
                printf("stack->length: %d\n", stack->length);
                if (stack->length && stack->top->value != '(') {
                    error_score += 3;

                    scanf("%s\n"); // ignore the rest of the line

                    free_stack_node(stack->top);
                    stack->top = NULL;
                    stack->length = 0;
                } else {
                    stack_pop(stack);
                }
                
                break;
            }
            case ']': {
                printf("stack->length: %d\n", stack->length);
                if (stack->length && stack->top->value != '[') {
                    error_score += 57;

                    scanf("%s\n"); // ignore the rest of the line

                    free_stack_node(stack->top);
                    stack->top = NULL;
                    stack->length = 0;
                } else {
                    stack_pop(stack);
                }
                
                break;
            }
            case '}': {
                printf("stack->length: %d\n", stack->length);
                if (stack->length && stack->top->value != '{') {
                    error_score += 1197;

                    scanf("%s\n"); // ignore the rest of the line

                    free_stack_node(stack->top);
                    stack->top = NULL;
                    stack->length = 0;
                } else {
                    stack_pop(stack);
                }
                
                break;
            }
            case '>': {
                printf("stack->length: %d\n", stack->length);
                if (stack->length && stack->top->value != '<') {
                    error_score += 25137;

                    scanf("%s\n"); // ignore the rest of the line

                    free_stack_node(stack->top);
                    stack->top = NULL;
                    stack->length = 0;
                } else {
                    stack_pop(stack);
                }
                
                break;
            }

        }
    }

    printf("total error score: %u\n", error_score);

    free(stack);
    return 0;
}
