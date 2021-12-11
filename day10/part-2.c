/* @reueljonathan 2021-12-10:
 *
 * This code is messy, but the idea is quite simple:
 *
 * - Create a stack of opening characters. ( [ { <
 * - Read each character until EOF
 * - check the read character:
 *   - if it is an openning character, push it to stack
 *   - if it is a closing character:
 *      - if the character at the top of the stack is NOT the associated (part 1 of puzzle)
 *        opening char (e.g read ')' but in the top of the stack was found a '<')
 *          - line is corrupted
 *          - sum specific error value of closing character to error_score
 *          - discard the rest of line
 *          - go to next line
 *      - if the character at the top of the stack is the associated
 *        closing character:
 *          - pop from the stack.
 *   
 *   - if the stack still has elements (length > 0):
 *      - pop it until length == 0
 *          - sum the value of the correspondent closing character to 
 *            completion_score
 *          - insert completion_score in the "dynamic array" completion_scores
 *
 *
 *   - sort completion_scores array,
 *   - print the element at the middle (part 2 of the puzzle)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct dynamic_int_array {
    unsigned long int * arr;
    size_t length;
    size_t capacity;
};

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

int compare_ints(const void * a, const void * b) {
    unsigned long int ua = *(unsigned long int *)a;
    unsigned long int ub = *(unsigned long int *)b;

    printf("comparing %lu and %lu\n", ua, ub);

    if (ua > ub) return 1;

    return -1 ;
}

int main() {
    int c;
    struct stack * stack = create_stack();
    unsigned int error_score = 0;
    struct dynamic_int_array completion_scores;

    completion_scores.length = 0;
    completion_scores.capacity = 1;
    completion_scores.arr = NULL;

    assert(stack->length == 0);
    assert(stack->top == NULL);

    while ((c = getc(stdin)) != EOF) {
        print_stack(stack);

        switch (c) {
            case '\n': {
                struct stack_node * node;
                unsigned long int completion_score = 0;

                if (stack->length > 0) {
                    while(stack->length) {
                        node = stack_pop(stack); 

                        switch (node->value) {
                            case '(': // we should close with ')'. Add 1 to completion score.
                                completion_score = (completion_score * 5) + 1;
                                break;
                            case '[': // we should close with ')'. Add 1 to completion score.
                                completion_score = (completion_score * 5) + 2;
                                break;
                            case '{': // we should close with ')'. Add 1 to completion score.
                                completion_score = (completion_score * 5) + 3;
                                break;
                            case '<': // we should close with ')'. Add 1 to completion score.
                                completion_score = (completion_score * 5) + 4;
                                break;

                        }


                        free(node);
                    }
                }

                completion_scores.length++; 

                if (completion_scores.length >= completion_scores.capacity) {
                    completion_scores.capacity *= 2;
                    completion_scores.arr =
                        (unsigned long int *) realloc(completion_scores.arr, sizeof(unsigned long int) * completion_scores.capacity);
                }
                completion_scores.arr[completion_scores.length - 1] = completion_score;
                printf("completion score: %lu\n", completion_score);


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

    assert(completion_scores.arr == &completion_scores.arr[0]);


    qsort(completion_scores.arr, completion_scores.length, sizeof(unsigned long int), compare_ints);

    printf("completion_scores: len = %d, cap = %d\n", completion_scores.length, completion_scores.capacity);
    printf("[\n");
    for (int i = 0; i < completion_scores.length; i++) {
        printf("[%2d] - %15lu\n", i, completion_scores.arr[i]);
    }
    printf("]\n");
    printf("winner complete score: %lu\n", completion_scores.arr[completion_scores.length / 2]);
    printf("total error score: %u\n", error_score);

    free(stack);
    return 0;
}
