#include <stdlib.h>
#include <stdio.h>
#include "char_list.h"

struct char_list * create_char_list() {
    struct char_list * list = (struct char_list *) malloc(sizeof(struct char_list));

    list->length = 0;
    list->root   = NULL;
    list->last   = NULL;

    return list;
}

void free_node(struct char_node * node) {
    if (node == NULL) return;

    free_node(node->next);
    free(node);
}

void free_list(struct char_list * list) {
    free_node(list->root);
    free(list);
}

struct char_node * add_char_node(struct char_list * list, char c) {
    struct char_node * node;

    node = (struct char_node *) malloc(sizeof(struct char_node));

    node->value = c;
    node->next  = NULL;

    if (list->root == NULL) {
        list->root = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }

    list->length++;

    return node;
}

void print_char_list(struct char_list * list) {
    struct char_node * node = list->root;

    while (node) {
        printf("%c", node->value);

        node = node->next;
    }
    puts("");
}
