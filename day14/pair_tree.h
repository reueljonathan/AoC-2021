#include "char_list.h"

struct pair_node {
    char to_insert;
    char pair[3];
    struct pair_node * left;
    struct pair_node * right;
};

struct pair_tree {
    struct pair_node * root;
};

struct pair_tree * create_pair_tree();
void free_pair_node(struct pair_node * node);
void free_pair_tree(struct pair_tree * tree);
//void print_pair_list(struct pair_list * list);
void transform_char_list(struct char_list * char_list, struct pair_tree * pair_tree);

struct pair_node *
add_pair_node(struct pair_tree * tree, char * pair, char char_to_insert);

struct pair_node *
find (struct pair_tree * tree, char * pair);
