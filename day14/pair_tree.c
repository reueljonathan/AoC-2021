#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pair_tree.h"
#include "char_list.h"

struct pair_tree * create_pair_tree() {
    struct pair_tree * tree = (struct pair_tree *) malloc(sizeof(struct pair_tree));

    tree->root = NULL;

    return tree;
}

void free_pair_node(struct pair_node * node) {
    if (node == NULL) return;

    free(node->left);
    free(node->right);
    free(node);
}

void free_pair_tree(struct pair_tree * tree) {
    free_pair_node(tree->root);
    free(tree);
}

struct pair_node *
add_pair_node(struct pair_tree * tree, char * pair, char char_to_insert) {
    struct pair_node * node = (struct pair_node *) malloc(sizeof(struct pair_node));
    struct pair_node * aux;
    int inserted_node = 0;

    // the most correct approach should be check if the pair really has size 2 + '\0';
    memcpy(node->pair, pair, 3);
    node->to_insert = char_to_insert;
    node->left = node->right = NULL;
    
    if (tree->root == NULL) {
        tree->root = node;
    } else {
        aux = tree->root;

        while (!inserted_node) {
            if (strcmp(node->pair, aux->pair) > 0) { // go to right
                if (aux->right == NULL) {
                    aux->right = node;
                    inserted_node = 1;
                } else {
                    aux = aux->right;
                }
            } else { // go to left
                if (aux->left == NULL) {
                    aux->left = node;
                    inserted_node = 1;
                } else {
                    aux = aux->left;
                }
            }
        }
    }
}

void transform_char_list(struct char_list * char_list, struct pair_tree * pair_tree) {
    struct char_node * char_node = char_list->root;
    struct pair_node * pair_node;
    struct char_node * new_node;
    char pair[3];
    int comparison;

    int steps_to_find_pair;

    pair[2] = '\0';
    while (char_node && char_node->next) {
        pair[0] = char_node->value;
        pair[1] = char_node->next->value;
        pair_node = pair_tree->root;

        steps_to_find_pair = 0;

        while (pair_node) {
            comparison = strcmp(pair, pair_node->pair);

            if (comparison == 0) { // found a pair!
                new_node = (struct char_node *) malloc(sizeof(struct char_node));

                new_node->value = pair_node->to_insert;
                new_node->next  = char_node->next;
                char_node->next = new_node;
                char_node = new_node->next;
                char_list->length++;

//                printf("%u steps to find pair!\n", steps_to_find_pair);
                //printf("char list size: %u\n", char_list->length);

                break;
            } else if (comparison > 0) {
                pair_node = pair_node->right;
                steps_to_find_pair++;
            } else {
                pair_node = pair_node->left;
                steps_to_find_pair++;
            }
        }
    }
}

struct pair_node *
find (struct pair_tree * tree, char * pair) {
    struct pair_node * node = tree->root;
    int comparison;

    while (node) {
        comparison = strcmp(pair, node->pair);

        if (comparison == 0) {
            return node;
        } else if (comparison > 0) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    return NULL;
}
