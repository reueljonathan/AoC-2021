#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pair_list.h"
#include "char_list.h"

struct pair_list * create_pair_list() {
    struct pair_list * list = (struct pair_list *) malloc(sizeof(struct pair_list));

    list->root   = NULL;
    list->last   = NULL;
    list->length = 0;

    return list;
}

void free_pair_node(struct pair_node * node) {
    if (node == NULL) return;

    free(node->next);
    free(node);
}

void free_pair_list(struct pair_list * list) {
    free_pair_node(list->root);
    free(list);
}

struct pair_node *
add_pair_node(struct pair_list * list, char * pair, char char_to_insert) {
    struct pair_node * node = (struct pair_node *) malloc(sizeof(struct pair_node));

    // the most correct approach should be check if the pair really has size 2 + '\0';
    memcpy(node->pair, pair, 3);
    node->to_insert = char_to_insert;
    node->next = NULL;
    
    if (list->root == NULL) {
        list->root = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }

    list->length++;
}

void print_pair_list(struct pair_list * list) {
    struct pair_node * node = list->root;

    while (node) {
        printf("%s (%c) -> ", node->pair, node->to_insert); 
        node = node->next;
    }
    puts("NULL");
}

void transform_char_list(struct char_list * char_list, struct pair_list * pair_list) {
    struct char_node * char_node = char_list->root;
    struct pair_node * pair_node;
    struct char_node * new_node;

    while (char_node && char_node->next) {

        pair_node = pair_list->root;
        while (pair_node) {
            //char_node = char_list->root;
        
            if (char_node->value == pair_node->pair[0] &&
                char_node->next->value == pair_node->pair[1]) { // found ocurrence of pair
                //printf("found pair %s. ", pair_node->pair);
                
                new_node = (struct char_node *) malloc(sizeof(struct char_node));

                new_node->value = pair_node->to_insert;
                new_node->next  = char_node->next;
                char_node->next = new_node;
                char_node = new_node->next;
                char_list->length++;

                break;
            }

            pair_node = pair_node->next;
        }

        //print_char_list(char_list);
    }
}
