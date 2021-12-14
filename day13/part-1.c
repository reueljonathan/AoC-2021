#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG 0

struct point {
    unsigned x, y;
    struct point * next;
};

struct point_list {
    struct point * root;
    struct point * last; //optmize push
};

void add_point(struct point_list * list, unsigned x, unsigned y) {
    struct point * new_point = (struct point *) malloc(sizeof(struct point));

    new_point->x    = x;
    new_point->y    = y;
    new_point->next = NULL;

    if (list->root == NULL) {
        list->root = new_point;
        list->last = new_point;
    } else {
        list->last->next = new_point;
        list->last = list->last->next;
    }
}

void print_list(struct point_list * list) {
    struct point * point = list->root;
    unsigned printed_in_current_line = 0;

    printf("point list:\n");
    while (point) {
        printf("(%4u, %4u) -> ", point->x, point->y);
        printed_in_current_line++;

        if (printed_in_current_line == 10) {
            printf("\n");
            printed_in_current_line = 0;
        }

        point = point->next;
    }
    printf("NULL\n");
}

void free_list_point(struct point * point) {
    if (point == NULL) return;

    free_list_point(point->next);
    free(point);
}

void free_list(struct point_list * list) {
    free_list_point(list->root);
    free(list);
}

struct point_list * create_list() {
    struct point_list * list = (struct point_list *) malloc(sizeof(struct point_list));
    list->root = NULL;
    list->last = NULL;

    return list;
}

void merge_duplicated_nodes(struct point_list * list) {
    struct point * point = list->root;
    struct point * other_point;
    struct point * aux;

    while (point) {
        other_point = point->next;

        while (other_point &&
            point->x == other_point->x &&
            point->y == other_point->y) {
#if DEBUG
            printf("found two nodes with values { x: %u, y: %u }\n", point->x, point->y);
#endif
            aux = other_point;

            point->next = other_point->next;
            other_point = point->next;
            free(aux);
        }

        while (other_point && other_point->next) {
            if ((point->x == other_point->next->x) &&
                (point->y == other_point->next->y)) {
#if DEBUG
                printf("found two nodes with values { x: %u, y: %u }\n", point->x, point->y);
#endif
                aux = other_point->next;

                other_point->next = other_point->next->next;

                free(aux);
            }

            other_point = other_point->next;
        }
        
        point = point->next;
    }
}

void fold_X(struct point_list * list, unsigned fold) {
    struct point * point = list->root;

    while (point) {

        if (point->x > fold) {
            point->x = fold - (point->x - fold);
        }

        point = point->next;
    }

    merge_duplicated_nodes(list);
}

void fold_Y(struct point_list * list, unsigned fold) {
    struct point * point = list->root;

    while (point) {

        if (point->y > fold) {
            unsigned previous_y = point->y;
#if DEBUG
            printf("found node to fold along Y = %u: ", fold);
            printf("{ x: %4u, y: %4u } -> ", point->x, point->y);
#endif

            point->y = fold - (point->y - fold);

#if DEBUG
            printf("{ x: %4u, y: %4u }\n", point->x, point->y);
            assert(point->y <= previous_y);
#endif
        }

        point = point->next;
    }

    merge_duplicated_nodes(list);
}

size_t count_list_nodes(struct point_list * list) {
    struct point * point = list->root;
    size_t size = 0;

    while (point) {
        size++;
        point = point->next;
    }

    return size;
}

int main() {
    unsigned x, y, fold_point;
    char fold_direction;
    struct point_list * list = create_list();

    while (scanf("%u, %u\n", &x, &y) > 0) {
        add_point(list, x, y);
    }
    //print_list(list); // debug purpose =)
    printf("Initial list size: %u\n", count_list_nodes(list));

    while (scanf("fold along %c=%u\n", &fold_direction, &fold_point) > 0) {

        if (fold_direction == 'x') {
            printf("folding along X = %u\n", fold_point);
            fold_X(list, fold_point);
            //print_list(list);
        } else if (fold_direction == 'y') {
            printf("folding along Y = %u\n", fold_point);
            fold_Y(list, fold_point);
            //print_list(list);
        }

        printf("Current list size: %u\n", count_list_nodes(list));
    }

    printf("list size: %u\n", count_list_nodes(list));

    free_list(list);
    return 0;
}
