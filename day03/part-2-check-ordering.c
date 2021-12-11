#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_STR_SIZE 12

int compare_numbers(const void * a, const void * b) {
    printf("value of a: %p, value of b: %p\n", a, b);
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

int main() {
    char ** numbers = NULL;
    size_t numbers_size = 0;
    char number_str[BINARY_STR_SIZE + 1];

    while (scanf("%s", number_str) != EOF) {
        numbers_size++;

        numbers = (char **) realloc(numbers, sizeof(char *) * numbers_size); 
        numbers[numbers_size - 1] = NULL;
        numbers[numbers_size - 1] = (char *) realloc(numbers[numbers_size - 1], sizeof(char) * (BINARY_STR_SIZE + 1));

        memcpy(numbers[numbers_size - 1], number_str, (BINARY_STR_SIZE + 1));
    }

    printf("Printing all read numbers (%d):\n", numbers_size);
    for (int i = 0; i < numbers_size; i++) {
        printf("%p, %p - %s\n", &numbers[i], &numbers[i][0], numbers[i]);
    }

    puts("sorting string representations of the numbers...");
    qsort(numbers, numbers_size, sizeof(char *), compare_numbers); 

    puts("printing sorted array of numbers");
    for (int i = 0; i < numbers_size; i++) {
        printf("%2d - %s\n", i, numbers[i]);
    }

    // clear memory
    for (int i = 0; i < numbers_size; i++) {
        free(numbers[i]);
    }
    free(numbers);

    return 0;
}
