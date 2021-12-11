#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct crab_submarine_group {
    unsigned int position;
    unsigned int num_submarines;
    struct crab_submarine_group * next;
};

struct crab_submarine_list {
    struct crab_submarine_group * root;
};

void add_submarine_to_list(struct crab_submarine_list * list, unsigned int crab_position) {
    struct crab_submarine_group * node;

    if (list->root == NULL) {
        list->root = (struct crab_submarine_group *) malloc(sizeof(struct crab_submarine_group));
        list->root->position = crab_position;
        list->root->num_submarines = 1;
        list->root->next = NULL;
    } else {
        node = list->root;

        if (crab_position == node->position) {
            node->num_submarines++;
            return;
        }

        while (node->next != NULL) {
            if (crab_position == node->next->position) {
                node->next->num_submarines++;
                return;
            } else {
                node = node->next; 
            }
        }

        node->next = (struct crab_submarine_group *) malloc(sizeof(struct crab_submarine_group));

        node->next->position = crab_position;
        node->next->num_submarines = 1;
        node->next->next = NULL;
    }
}

void print_list(struct crab_submarine_list * list) {
    struct crab_submarine_group * node = list->root;

    while (node != NULL) {
        printf("{ position: %4u, num_submarines: %4u }\n", node->position, node->num_submarines);
        node = node->next;
    }
}

void free_list_node(struct crab_submarine_group * node) {
    if (node == NULL) return;

    free_list_node(node->next);
    free(node);
}

void free_list(struct crab_submarine_list * list) {
    free_list_node(list->root);
    free(list);
}

int main() {
    int crab_position;
    unsigned long lowest_needed_fuel = ULONG_MAX, fuel_quantity;
    struct crab_submarine_list * list = (struct crab_submarine_list *) malloc(sizeof(struct crab_submarine_list));
    struct crab_submarine_group * nodeA, * nodeB;

    list->root = NULL;

    while (scanf("%u,", &crab_position) != EOF) {
        add_submarine_to_list(list, crab_position); 
    }

    print_list(list);

    nodeA = list->root;
    while (nodeA != NULL) {
        nodeB = list->root;
        fuel_quantity = 0;
        int a_pos = (int) nodeA->position;

        printf("nodeA - pos %u:\n", nodeA->position);
        while (nodeB != NULL) {
            printf("\tnodeB - pos %u, num submarines: %u:\n", nodeB->position, nodeB->num_submarines);
            fuel_quantity += abs(a_pos - (int) nodeB->position) * nodeB->num_submarines;
            printf("\tpartial fuel sum: %u\n", fuel_quantity);

            nodeB = nodeB->next;
        }

        if (fuel_quantity < lowest_needed_fuel) {
            lowest_needed_fuel = fuel_quantity;
        }

        nodeA = nodeA->next;
    }

    printf("less ammount of fuel needed to line up: %lu fuel.\n", lowest_needed_fuel);
    
    free_list(list);
    return 0;
}
