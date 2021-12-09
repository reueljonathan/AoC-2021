#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_STR_SIZE 12

int compare_numbers(const void * a, const void * b) {
    printf("address of a: %p, address of b: %p\n", a, b);
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
        printf("%s\n", numbers[i]);
    }

    puts("sorting string representations of the numbers...");
    qsort(numbers, numbers_size, sizeof(char *), compare_numbers); 

    puts("printing sorted array of numbers");
    for (int i = 0; i < numbers_size; i++) {
        printf("%2d - %s\n", i, numbers[i]);
    }

    //search for oxygen generator rating
    int initial_index = 0, final_index = numbers_size - 1;
    int bit_position = 0;

    printf("numbers size: %d\n", numbers_size);

    while (initial_index != final_index) {
        printf("initial index: %d, final index: %d\n", initial_index, final_index);
        int number_of_zeroes = 0;
        int chunk_size = 0;

        for (int i = initial_index; i <= final_index; i++) {
            chunk_size++;
            if (numbers[i][bit_position] == '0')
                number_of_zeroes++;
        }

        printf("number of zeroes: %d\n", number_of_zeroes);

        if (number_of_zeroes > chunk_size / 2) {
            final_index = initial_index + number_of_zeroes - 1;
        } else {
            initial_index += number_of_zeroes;
        }

        bit_position++;
    }

    printf("oxygen generator rating: %s\n", numbers[final_index]);

    int oxygen_generator_rating = strtol(numbers[final_index], NULL, 2);

    //search for CO2 scrubber rating
    initial_index = 0, final_index = numbers_size - 1;
    bit_position = 0;

    printf("numbers size: %d\n", numbers_size);

    while (initial_index != final_index) {
        printf("initial index: %d, final index: %d\n", initial_index, final_index);
        int number_of_zeroes = 0;
        int chunk_size = 0;

        for (int i = initial_index; i <= final_index; i++) {
            chunk_size++;
            if (numbers[i][bit_position] == '0')
                number_of_zeroes++;
        }

        printf("number of zeroes: %d\n", number_of_zeroes);

        if (number_of_zeroes > chunk_size / 2) {
            initial_index += number_of_zeroes;
        } else {
            final_index = initial_index + number_of_zeroes - 1;
        }

        bit_position++;
    }

    printf("C02 scrubber rating: %s\n", numbers[final_index]);
    int CO2_scrubber_rating = strtol(numbers[final_index], NULL, 2);

    printf("life supporting rating: %d\n", oxygen_generator_rating * CO2_scrubber_rating);

    // clear memory
    for (int i = 0; i < numbers_size; i++) {
        free(numbers[i]);
    }
    free(numbers);

    return 0;
}
