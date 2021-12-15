#include <stdio.h>
#include <stdlib.h>

#include "char_list.h"
#include "pair_list.h"

int compare(const void * a, const void * b) {
    return (*(int *) b) - (*(int *) a);
}

int main() {

    int c;
    char char_to_insert, pair[3];
    struct char_list * list = create_char_list();
    struct pair_list * pair_list = create_pair_list();
    unsigned int presence[25];

    while ((c = getc(stdin)) != '\n') {
        add_char_node(list, (char) c);
    }
    print_char_list(list);

    scanf("\n"); // ignore empty line

    while (scanf("%s -> %c\n", pair, &char_to_insert) != EOF) {
        printf("read pair '%s' that includes char '%c'\n", pair, char_to_insert);

        add_pair_node(pair_list, pair, char_to_insert);
    }
    print_pair_list(pair_list);

    for (int i = 0; i < 10; i++) {
        printf("- transformation #%u\n", i + 1);
        transform_char_list(list, pair_list);
    }

    //solution
    for (int i = 0; i < 25; i++) {
        presence[i] = 0;
    }
    struct char_node * char_node = list->root;
    while (char_node) {
        presence[char_node->value - 'A'] += 1;
        char_node = char_node->next;
    }
    qsort(presence, 25, sizeof(unsigned int), compare);

    printf("presences: [ ");
    for (int i = 0; i < 25; i++) {
        printf("%u ", presence[i]);
    }
    printf("]\n");

    free_pair_list(pair_list);
    free_list(list);
    return 0;
}
