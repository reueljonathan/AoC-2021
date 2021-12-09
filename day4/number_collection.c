#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "number_collection.h"

void print_num_collection(struct number_collection * collection) {
    printf("- collection: %p\n", collection);
    printf("- length: %d\n", collection->length);
    printf("[ ");
    for (int i = 0, len = collection->length; i < len; i++) {
        printf("%d ", collection->items[i]);
    }
    puts("]");
}

struct number_collection * create_number_collection() {
    struct number_collection * collection = (struct number_collection *) malloc(sizeof(struct number_collection));

    assert(collection != NULL);

    collection->length = 0;
    collection->items  = NULL;

    printf("address of newly created collection: %p\n", &collection);

    return collection;
}

void add_item(struct number_collection * num_collection, int number) {
    int * resized = (int *) realloc(num_collection->items, sizeof(int) * (num_collection->length + 1));

    if (resized) {
        num_collection->items = resized;
        num_collection->items[num_collection->length++] = number;
    }
}
