#ifndef CHAR_LIST_H
#define CHAR_LIST_H

struct char_node {
    char value;
    struct char_node * next;
};

struct char_list {
    size_t length;
    struct char_node * root;
    struct char_node * last;
};


struct char_list * create_char_list();
void free_node(struct char_node * node);
void free_list(struct char_list * list);
struct char_node * add_char_node(struct char_list * list, char c);
void print_char_list(struct char_list * list);

#endif
