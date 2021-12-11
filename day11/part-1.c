#include <stdio.h>
#include <stdbool.h>

#define NUM_STEPS 100

struct dumbo {
    bool flashed;
    unsigned int energy;
};

struct dumbo_collection {
    struct dumbo matrix[10][10];
    unsigned int num_flashes;
};

void print_matrix(const struct dumbo_collection * collection) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (collection->matrix[i][j].energy == 0) {
                printf("\033[1;32m%2u\033[0m ", collection->matrix[i][j].energy);
            } else {
                printf("%2u ", collection->matrix[i][j].energy);
            }
        }
        puts("");
    }
}

void propagate_flashes(struct dumbo_collection * collection, int i, int j, bool increase_energy) {
    if (i <   0 ||
        i >= 10 ||
        j <   0 ||
        j >= 10) {
        return; //out of bounds
    }

    if (!collection->matrix[i][j].flashed && increase_energy) {
        collection->matrix[i][j].energy++;
    }

    if (collection->matrix[i][j].energy > 9) { // flashes
        collection->matrix[i][j].flashed = true;
        collection->num_flashes++;
        collection->matrix[i][j].energy = 0;

        propagate_flashes(collection, i - 1, j, true);
        propagate_flashes(collection, i + 1, j, true);
        propagate_flashes(collection, i, j - 1, true);
        propagate_flashes(collection, i, j + 1, true);

        // diagonals
        propagate_flashes(collection, i - 1, j - 1, true);
        propagate_flashes(collection, i - 1, j + 1, true);
        propagate_flashes(collection, i + 1, j - 1, true);
        propagate_flashes(collection, i + 1, j + 1, true);
    }
}


int main() {
    
    struct dumbo_collection collection;
    collection.num_flashes = 0;

    printf("sizeof(collection): %d\n", sizeof(collection));

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int c = getc(stdin);
            
            collection.matrix[i][j].energy = c - '0';
        }
        getc(stdin); // ignore \n
    }

    printf("read matrix\n");
    print_matrix(&collection);

    for (int s = 0; s < NUM_STEPS; s++) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                collection.matrix[i][j].energy++;
            }
        }

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (collection.matrix[i][j].energy > 9) { // flashes
                    propagate_flashes(&collection, i, j, false);
                }
            }
        }

        printf("STEP %d:\n", s + 1);
        print_matrix(&collection);

        // reset flashed status for next step
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                collection.matrix[i][j].flashed = false;
            }
        }
    }

    printf("num flashes in %d steps: %u\n", NUM_STEPS, collection.num_flashes);

}
