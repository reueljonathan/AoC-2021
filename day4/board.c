#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"

struct board * create_bingo_board() {
    struct board * board = (struct board *) malloc(sizeof(struct board));
    struct board_number * number;

    assert(board != NULL);

    // added for part 2 solution
    board->discarted = false;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            struct board_number number;
            number.drawn = false;
            number.value = 0;
            board->numbers[i][j] = number;
        }
    }

    return board;
}

void read_single_board(struct board * board) {
    
}

struct board * read_boards(int * size) {
    int number;
    int character;
    struct board * boards = NULL;
    (*size) = 0;

    while ((character = getc(stdin)) != EOF) {
        //printf("read char %d -> '%c'\n", character, character);
        struct board new_board;
        (*size) += 1;
        //printf("next size: %d\n", sizeof(struct board *) * (*size));
        boards = (struct board *) realloc(boards, sizeof(struct board) * (*size));

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                scanf("%d", &new_board.numbers[i][j].value);
                new_board.numbers[i][j].drawn = false;
                //printf("board #%d [%d][%d]: %d\n", (*size), i, j, new_board.numbers[i][j].value);
            }
        }

        boards[(*size) - 1] = new_board;

        //printf("printing board %d:\n", *size);
        //print_board(&(boards[(*size) - 1]));
    }

    return boards;
}

void free_board(struct board * board) {
    free(board);
}

void print_board(struct board * board) {
    if (board != NULL) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                printf("%02d ", board->numbers[i][j].value);

                if (board->numbers[i][j].drawn) {
                    printf("[X]");
                } else {
                    printf("[ ]");
                }

                if (j == 4) {
                    printf("\n");
                } else {
                    printf(" ");
                }
            }
        }
    }
}
