#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct map {
    int ** heights;
    int rows, cols;
    int rows_capacity, cols_capacity;
};

unsigned int calculate_basin_length(int ** heights, int ** visited_heights, int previous_height, int i, int j, int rows, int cols) {
    if (i < 0 || i >= rows)                    return 0;
    else if (j < 0 || j >= cols)               return 0;
    else if (visited_heights[i][j])            return 0;
    else if (heights[i][j] <= previous_height) return 0;
    else if (heights[i][j] == 9)               return 0;
    else {
        visited_heights[i][j] = 1;

        /* print visited nodes and "visitation map" */
        /*
        printf("visited %d at (%d,%d)\n", heights[i][j], i, j);

        for (int ii = 0; ii < rows; ii++) {
            for (int jj = 0; jj < cols; jj++) {
                if (visited_heights[ii][jj]) printf("%d", heights[ii][jj]);
                else printf(".");
            }
            puts("");
        }
        */

        return 1 
            + calculate_basin_length(heights, visited_heights, heights[i][j], i - 1, j, rows, cols)
            + calculate_basin_length(heights, visited_heights, heights[i][j], i + 1, j, rows, cols)
            + calculate_basin_length(heights, visited_heights, heights[i][j], i, j + 1, rows, cols)
            + calculate_basin_length(heights, visited_heights, heights[i][j], i, j - 1, rows, cols);
    }

}

int main() {

    struct map map;
    int ** visited_heights;

    map.rows = map.cols = 0;
    map.rows_capacity = map.cols_capacity = 1;
    map.heights = (int **) malloc(sizeof(int *));
    map.heights[0] = (int *) malloc(sizeof(int));

    int c, row = 0, column = 0;
    unsigned long risk_sum = 0;

    unsigned long greatest_basin, second_greatest_basin, third_greatest_basin, len_basin;

    greatest_basin = second_greatest_basin = third_greatest_basin = 0;

    while ((c = getc(stdin)) != EOF) {
        if (c == '\n') {
            row++;
            column = 0;

            if (row > map.rows) {
                //printf("increasing map.rows from %d to %d\n", map.rows, row);
                map.rows = row;
            }

            if (row >= map.rows_capacity) {
                //printf("increasing map.rows_capacity from %d", map.rows_capacity);
                map.rows_capacity *= 2;
                //printf(" to %d\n", map.rows_capacity);
                map.heights = (int **) realloc(map.heights, sizeof(int *) * map.rows_capacity);
            }

            map.heights[row] = (int *) malloc(sizeof(int) * map.cols_capacity);    
            
        } else {
            map.heights[row][column] = c - '0';
            assert(map.heights[row][column] == c - '0');
            column++;

            if (column > map.cols) {
                //printf("increasing map.cols from %d to %d\n", map.cols, column);
                map.cols = column;
            }

            if (column >= map.cols_capacity) {
                //printf("increasing map.cols_capacity from %d", map.cols_capacity);
                map.cols_capacity *= 2;
                //printf(" to %d\n", map.cols_capacity);
                map.heights[row] = (int *) realloc(map.heights[row], sizeof(int) * map.cols_capacity);
            }
        }
    }

    visited_heights = (int **) malloc(sizeof(int *) * map.rows);
    for (int i = 0; i < map.rows; i++) {
        visited_heights[i] = (int *) calloc(map.cols, sizeof(int));
    }

    /*
    printf("printing read array\n");
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            printf("%d", map.heights[i][j]);
        }
        puts("");
    }
    */

    printf("calculating risks...\n");
    /*
    if (map.rows > 1 && map.cols > 1) {
        int upper_left_corner   = map.heights[0][0];
        int upper_right_corner  = map.heights[0][map.cols-1];
        int bottom_left_corner  = map.heights[map.rows - 1][0];
        int bottom_right_corner = map.heights[map.rows - 1][map.cols - 1];

        if (upper_left_corner < map.heights[0][1] && upper_left_corner < map.heights[1][0]) {
            printf("%d + 1\n", upper_left_corner);
            risk_sum += upper_left_corner + 1;
        }

        if (upper_right_corner < map.heights[0][map.cols - 2] && upper_right_corner < map.heights[1][map.cols - 1]) {
            printf("%d + 1\n", upper_right_corner);
            risk_sum += upper_right_corner + 1;
        }

        if (bottom_left_corner < map.heights[map.rows - 1][1] && bottom_left_corner < map.heights[map.rows - 2][0]) {
            printf("%d + 1\n", bottom_left_corner);
            risk_sum += bottom_left_corner + 1;
        }

        if (bottom_right_corner < map.heights[map.rows - 1][map.cols - 2] && bottom_right_corner < map.heights[map.rows - 2][map.cols - 1]) {
            printf("%d + 1\n", bottom_right_corner);
            risk_sum += bottom_right_corner + 1;
        }
    }
    */

    int height;
    int smaller_than_left, smaller_than_right, smaller_than_above, smaller_than_below;
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            smaller_than_left = 
            smaller_than_right = 
            smaller_than_above = 
            smaller_than_below = 1;

            height = map.heights[i][j];

            if (i > 0) {
                smaller_than_above = height < map.heights[i - 1][j];
            }

            if (i < map.rows - 1) {
                smaller_than_below = height < map.heights[i + 1][j];
            }

            if (j > 0) {
                smaller_than_left = height < map.heights[i][j - 1];
            }

            if (j < map.cols - 1) {
                smaller_than_right = height < map.heights[i][j + 1];
            }


            if (smaller_than_left &&
                smaller_than_right &&
                smaller_than_above &&
                smaller_than_below) {


                // part 1
                risk_sum += height + 1;

                // part 2
                visited_heights[i][j] = 1;
                len_basin = 1 +
                            calculate_basin_length(map.heights, visited_heights, height, i + 1, j, map.rows, map.cols) +
                            calculate_basin_length(map.heights, visited_heights, height, i - 1, j, map.rows, map.cols) +
                            calculate_basin_length(map.heights, visited_heights, height, i, j + 1, map.rows, map.cols) +
                            calculate_basin_length(map.heights, visited_heights, height, i, j - 1, map.rows, map.cols);

                //printf("len basin: %u\n", len_basin);

                if (len_basin > greatest_basin) {
                    third_greatest_basin = second_greatest_basin;
                    second_greatest_basin = greatest_basin;
                    greatest_basin = len_basin;
                } else if (len_basin > second_greatest_basin) {
                    third_greatest_basin = second_greatest_basin;
                    second_greatest_basin = len_basin;
                } else if (len_basin > third_greatest_basin) {
                    third_greatest_basin = len_basin;
                }
            }
        }
    }

    printf("sum of risks: %llu\n", risk_sum);
    printf("size of basins: %lu * %lu * %lu = %lu\n",
            greatest_basin,
            second_greatest_basin,
            third_greatest_basin,
            greatest_basin * second_greatest_basin * third_greatest_basin);

    return 0;
}
