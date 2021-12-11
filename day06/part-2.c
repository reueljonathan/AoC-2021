#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define TOTAL_DAYS 256

struct lanternfish_group {
    int days;
    unsigned long quantity; // number of lanternfishes in this group.
    struct lanternfish_group * next;
};

// I choose to solve this with a list just for practice, but probably could be a dynamic array
// and probably with dynamic arrays it is easier.
struct lanternfish_group_list {
    struct lanternfish_group * root;
    struct lanternfish_group * last;
};

struct lanternfish_group_list * create_fish_group_list() {
    struct lanternfish_group_list * result = (struct lanternfish_group_list *) malloc(sizeof(struct lanternfish_group_list)); 

    result->root = result->last = NULL;

    return result;
};

void add_initial_list_node(struct lanternfish_group_list * list, unsigned long num_days) {
    if (list->root == NULL) {
        list->root = (struct lanternfish_group *) malloc(sizeof(struct lanternfish_group));
        list->root->days = num_days;
        list->root->quantity = 1;
        list->root->next = NULL;
        list->last = list->root;
    } else {
        struct lanternfish_group * node = list->root;

        if (node->days == num_days) {
            node->quantity++;
            return;
        }
        

        while (node->next != NULL) {
            if (node->next->days == num_days) {
                node->next->quantity++;
                return;
            }

            node = node->next;
        }

        // node->next is NULL, so there is no node with days == num_days
        // we need to create a new one.
        node->next = (struct lanternfish_group *) malloc(sizeof(struct lanternfish_group));

        node->next->next = NULL;
        node->next->days = num_days;
        node->next->quantity = 1;
        list->last = node->next;
    }
}

void add_node(struct lanternfish_group_list * list, unsigned long num_fishes_in_group) {
    struct lanternfish_group * last;

    if (list->root == NULL) {
        list->root = (struct lanternfish_group *) malloc(sizeof(struct lanternfish_group));
        list->root->days = 8;
        list->root->quantity = num_fishes_in_group;
        list->root->next = NULL;
        list->last = list->root;
    } else {
        last = list->last;
        last->next = (struct lanternfish_group *) malloc(sizeof(struct lanternfish_group));
        last->next->days = 8;
        last->next->quantity = num_fishes_in_group;
        last->next->next = NULL;
        list->last = last->next;
    }
}

void free_fish_list(struct lanternfish_group * node) {
    if (node == NULL) return;

    free_fish_list(node->next);
    free(node);
}

void print_fish_list(struct lanternfish_group_list * list) {
    struct lanternfish_group * node = list->root;
    int quantity = 1;

    printf("list nodes:\n");
    while (node != NULL) {
        printf("%02d - { days: %d, quantity: %llu, next: %p }\n", 
                quantity++,
                node->days, 
                node->quantity,
                node->next);

        node = node->next;
    }
}


int main() {
    struct lanternfish_group_list * list = create_fish_group_list();
    unsigned long num, day = 0, num_created_fishes = 0;;
    struct lanternfish_group * node;


    while (scanf("%d,", &num) != EOF) {
        add_initial_list_node(list, num);
    }

    print_fish_list(list);

    while (day < TOTAL_DAYS) {
        num_created_fishes = 0;
        node = list->root;

        while (node != NULL) {
            node->days--;

            if (node->days == -1) {
                node->days = 6;
                num_created_fishes += node->quantity; // each fish generate a new one
            }

            node = node->next;
        }

        if (num_created_fishes > 0) {
            add_node(list, num_created_fishes);
        }

        day++;
    }

    unsigned long num_fishes = 0;
    node = list->root;

    while (node != NULL) {
        num_fishes += node->quantity;
        //printf("partial sum %llu\n", num_fishes);
        node = node->next;
    }

    print_fish_list(list);
    printf("nim fishes: %llu\n", num_fishes);
    /*
    printf("max: %llu\n", ULONG_MAX);
    printf("sizeof(int): %d bytes\n", sizeof(int));
    printf("sizeof(long long): %d bytes\n", sizeof(long long));
    printf("sizeof(unsigned long long): %d bytes\n", sizeof(unsigned long long));
    printf("sizeof(unsigned long long int): %d bytes\n", sizeof(unsigned long long int));
    */


    free_fish_list(list->root);
    free(list);
    printf("size of unsigned long long: %d\n", sizeof(unsigned long long));
    printf("size of unsigned long: %d\n", sizeof(unsigned long));
    return 0;

}
