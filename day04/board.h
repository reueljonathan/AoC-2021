#include <stdbool.h>

struct board_number {
    bool drawn;
    int  value;
};

struct board {
    struct board_number numbers[5][5];
    bool discarted;
};

struct board * create_bingo_board();
void read_single_board(struct board *);
struct board * read_boards(int * size);
void free_board(struct board *);
void print_board(struct board *);
