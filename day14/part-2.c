/* reueljonathan 2021-12-18:
 *
 * I confess! I searched for solutions after some days without
 * being able to solve this. And I've learned a lot. =)
 *
 * The code is a giant mess. I'll explain the solution here so I can remember
 * when I read this again (if ever...)
 *
 * So, the idea is the following:
 *
 * suppose the input example is:
 *
 * REUEL
 *
 * RE -> U
 * EL -> E
 * RU -> L
 *
 *
 * The code will do the following:
 *
 * 1. Read all characters from the polymer template and insert them into a linked list
 * of characters. Ex:
 *
 * char_list = 'R' -> 'E' -> 'U' -> 'E' -> 'L'
 *
 * 1.1. Increment the frequencies of each character in the polymer template in the
 * 'frequencies' array. It has 25 positions, each one for a letter.
 *
 * 2. For each 2 nodes in the char list, create a node in a [pair,frequency] list:
 *
 * frequency_list = ['RE', 1] -> ['EU', 1] -> ['UE', 1] -> ['EL', 1]
 *
 * 3. Read the following lines with the rules and save the rules in a binary tree. I
 * implemented the tree thinking that decreasing the number of checks of the rules
 * the code would run faster enough to solve part 2 fast. I was wrong. The problem is
 * really the exponential growth of the polymer (that I was trying to create).
 *
 *                          [ RE -> U ]
 *                         /           \
 *                   [ EL -> E ]   [ RU -> L ]
 *
 * 4. Repeat 40 times the following loop:
 *
 *      new_frequency_list = []
 *
 *      foreach node in frequency_list:
 *          rule = find_in_rule_tree(node->pair)
 *
 *          if (rule != NULL) // if we find a rule for this char pair.
 *              
 *              increase frequency of the char to be inserted
 *
 *              insert a new pair in new_frequency_list:
 *                  [ node->pair[0] + char_to_insert, node->frequency ]
 *
 *              insert a new pair in new_frequency_list:
 *                  [ char_to_insert + node->pair[1], node->frequency ]
 *              
 *
 *       return new_frequency_list 
 *
 * 5. After step 4, the 'frequencies' array will have the frequencies of 
 * each character in the final polymer.
 *
 * 6. Sort the 'frequencies' array.
 *
 * 7. Subtract the greatest frequency from the smallest non-zero frequency. (I did
 * this step outside of the code because I was tired and didn't wanted to implement this :P)
 *
 * 8. tada!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pair_tree.h" // changed list for tree in part 2 to speed up searchs
#include "char_list.h"


struct pair_frequency_node {
    unsigned long long frequency;
    char pair[3];
    struct pair_frequency_node * next;
};

struct pair_frequency_list {
    struct pair_frequency_node * root;
    struct pair_frequency_node * last;
};

struct pair_frequency_list * create_pair_frequency_list() {
    struct pair_frequency_list * list = 
        (struct pair_frequency_list *) malloc(sizeof(struct pair_frequency_list));

    list->root = NULL;
    list->last = NULL;

    return list;
};

void add_pair_frequency_node(
    struct pair_frequency_list * list,
    char * pair,
    unsigned long long frequency) {

    struct pair_frequency_node * node;

    node = list->root;

    while (node) {
        if (strcmp(node->pair, pair) == 0) {
            node->frequency += frequency;
            return;
        }

        node = node->next;
    }

    node = (struct pair_frequency_node *) malloc(sizeof(struct pair_frequency_node)); 
    node->frequency = frequency;
    node->next = NULL;
    memcpy(node->pair, pair, 3);

    
    if (list->root == NULL) {
        list->root = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }

}

void print_pair_frequency_list(struct pair_frequency_list * list) {
    struct pair_frequency_node * node;

    node = list->root;

    printf("[ ");
    while (node) {
        printf("%s (%u), ", node->pair, node->frequency);

        node = node->next;
    }
    printf("]\n");
}

void free_pair_frequency_node(struct pair_frequency_node * node) {
    if (node == NULL) return;

    free_pair_frequency_node(node->next);
    free(node);
}

void free_pair_frequency_list(struct pair_frequency_list * list) {
    free_pair_frequency_node(list->root);

    free(list);
}

int compare(const void * a, const void * b) {
    unsigned long long ula = (*(unsigned long long *) a);
    unsigned long long ulb = (*(unsigned long long *) b);

    if (ulb > ula) return 1;
    else if (ulb == ula) return 0;
    else return -1;

}

struct pair_frequency_list * expand_polymer(
    struct pair_frequency_list * list,
    struct pair_tree * rules_tree,
    unsigned long long * frequencies ){
    
    char pair_a[3];
    char pair_b[3];

    pair_a[2] = pair_b[2] = '\0';

    struct pair_frequency_list * new_list =
        (struct pair_frequency_list *) malloc(sizeof(struct pair_frequency_list));

    struct pair_frequency_node * node = list->root;

    struct pair_node * rule;

    while (node) {

        rule = find(rules_tree, node->pair);

        if (rule != NULL) {
            frequencies[rule->to_insert - 'A'] += node->frequency;

            pair_a[0] = node->pair[0]; 
            pair_a[1] = rule->to_insert; 

            pair_b[0] = rule->to_insert;
            pair_b[1] = node->pair[1];

            add_pair_frequency_node(new_list, pair_a, node->frequency); 
            add_pair_frequency_node(new_list, pair_b, node->frequency); 
        }

        node = node->next;
    }


    return new_list;
}

int main() {
    int c;
    char char_to_insert, pair[3];
    struct char_list * list = create_char_list();
    struct pair_tree * tree = create_pair_tree();
    struct pair_frequency_list * frequency_list = NULL;
    struct pair_frequency_list * aux;

    unsigned long long frequencies[25];

    //solution
    for (int i = 0; i < 25; i++) {
        frequencies[i] = 0;
    }

    while ((c = getc(stdin)) != '\n') {
        add_char_node(list, (char) c);
    }
    print_char_list(list);

    scanf("\n"); // ignore empty line

    while (scanf("%s -> %c\n", pair, &char_to_insert) != EOF) {
        add_pair_node(tree, pair, char_to_insert);
    }

    // calculate presence of characters in the template
    struct char_node * node;

    node = list->root;
    while (node) {
        frequencies[node->value - 'A']++;
        node = node->next;
    }

    // calculate the presence after insertions
    pair[2] = '\0';
    node = list->root;
    frequency_list = create_pair_frequency_list();

    while (node && node->next) {
        pair[0] = node->value;
        pair[1] = node->next->value;

        printf("pair to add: %s\n", pair);

        add_pair_frequency_node(frequency_list, pair, 1);

        node = node->next;
    }
    print_pair_frequency_list(frequency_list);

    for (int i = 0; i < 40; i++) {
        aux = frequency_list;

        frequency_list = expand_polymer(
                frequency_list,
                tree,
                frequencies);

        print_pair_frequency_list(frequency_list);
        free(aux);
    }

    qsort(frequencies, 25, sizeof(unsigned long long), compare);

    printf("frequencies: [ ");
    for (int i = 0; i < 25; i++) {
        printf("%llu ", frequencies[i]);
    }
    printf("]\n");

    free_pair_frequency_list(frequency_list);
    free_pair_tree(tree);
    free_list(list);
    return 0;
}
