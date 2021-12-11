#include <stdio.h>
#include <stdlib.h>

#define TOTAL_DAYS 80

int main() {

    // dynamic array simulating go slice (?) resize (i.e. doubling the size each time it becomes full.
    size_t size = 0;
    size_t capacity = 1;
    int * lantern_fishes = (int *) malloc(sizeof(int) * capacity);
    int num;
    int day;

    while (scanf("%d,", &num) != EOF) {
        lantern_fishes[size++] = num;

        if (size == capacity) {
            capacity *= 2;
            printf("new capacity: %d\n", capacity);
            lantern_fishes = (int *) realloc(lantern_fishes, sizeof(int) * capacity);
        }
    }
    printf("size after reading initial state: %d\n", size);

    day = 0;
    while (day < TOTAL_DAYS) {
        for (int i = 0, s = size; i < s; i++) {
            lantern_fishes[i]--;

            if (lantern_fishes[i] == -1) {
                lantern_fishes[i] = 6;
                if (size == capacity) {
                    capacity *= 2;
                    printf("new capacity: %d\n", capacity);
                    lantern_fishes = (int *) realloc(lantern_fishes, sizeof(int) * capacity);
                }

                lantern_fishes[size++] = 8;
            }
        }

        day++;
    }

    printf("num of lanternfishes: %d\n", size);

    return 0;

}
