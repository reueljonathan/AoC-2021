#include "char_list.h"

struct pair_node {
    char pair[3];
    char to_insert;
    struct pair_node * next;
};

struct pair_list {
    size_t length;
    struct pair_node * root;
    struct pair_node * last;
};

struct pair_list * create_pair_list();
void free_pair_node(struct pair_node * node);
void free_pair_list(struct pair_list * list);
void print_pair_list(struct pair_list * list);
void transform_char_list(struct char_list * char_list, struct pair_list * pair_list);

struct pair_node *
add_pair_node(struct pair_list * list, char * pair, char char_to_insert);

