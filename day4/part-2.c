#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "number_collection.h"
#include "board.h"

struct number_collection * read_bingo_numbers() {
    struct number_collection * num_collection;
    int number;

    //printf("num_collection before allocation: %p\n", num_collection);
    num_collection = create_number_collection();
    //printf("num_collection after allocation: %p\n", num_collection);
    
    //printf("printing empty number collection:\n");
    //print_num_collection(num_collection);

    // first number
    scanf("%d", &number);
    add_item(num_collection, number);
    while (scanf(",%d", &number) > 0) {
        add_item(num_collection, number);
    }

    printf("printing number collection:\n");
    print_num_collection(num_collection);

    return num_collection;
}

void free_num_collection(struct number_collection * collection) {
    free(collection->items);
    free(collection);
}

int main() {
    struct number_collection * bingo_numbers = read_bingo_numbers();
    int num_boards, current;
    struct board * boards = NULL;

    assert(bingo_numbers != NULL);

    boards = read_boards(&num_boards);
    assert(boards != NULL);

    /* WARNING: here is an awful amount of nested loops... sorry for that.
     * The logic is:
     * foreach random number
     *     check each number in each board
     *         if the random number is equal to the number in the board
     *             mark as checked
     *             check if all the numbers in the row or the column are checked.
     *                 if it is: BINGO!
     *
     * for part 2:
     * - added "discarted" flag to boards
     * - check only for non discarted boards
     * - print all until the last one
     */
    for (int n = 0; n < bingo_numbers->length; n++) {
        for (int b = 0; b < num_boards; b++) {
             
            if (!boards[b].discarted) {
            
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 5; j++) {
                        if (boards[b].numbers[i][j].value == bingo_numbers->items[n]) {
                            boards[b].numbers[i][j].drawn = true;

                            bool bingo_row = true;
                            bool bingo_col = true;

                            for (int k = 0; k < 5; k++) {
                                bingo_row = bingo_row && boards[b].numbers[i][k].drawn;
                                bingo_col = bingo_col && boards[b].numbers[k][j].drawn;
                            }

                            if (bingo_row || bingo_col) {
                                boards[b].discarted = true;
                                printf("##################### BINGO! #####################\n");
                                printf("Last drawn number: %d\n", bingo_numbers->items[n]);
                                print_board(&boards[b]);
                                //return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    free_num_collection(bingo_numbers);
}
