#include <stdio.h>
#include <stdlib.h>

#define DIAGRAM_SIDE 1000

int max(int a, int b) {
    if (a > b)
        return a;

    return b;
}

int min(int a, int b) {
    if (a < b)
        return a;

    return b;
}

struct point {
    int x;
    int y;
};

void print_map(int ** map) {
    for (int i = 0; i < DIAGRAM_SIDE; i++) {
        for (int j = 0; j < DIAGRAM_SIDE; j++) {
            if (map[i][j] == 0) {
                printf(".");
            } else {
                printf("%d", map[i][j]);
            }
        }
        puts("");
    }
}

int main() {
    // Initialize map (int matrix)
    int ** map = (int **) calloc(DIAGRAM_SIDE, sizeof(int *));
    for (int i = 0; i < DIAGRAM_SIDE; i++){ 
        map[i] = (int *) calloc(DIAGRAM_SIDE, sizeof(int));
    }

    struct point start, end;
    while (scanf("%d,%d -> %d,%d\n", &start.x, &start.y, &end.x, &end.y) != EOF) {

        /*
        printf("start point: %d, %d\n", start.x, start.y);
        printf("end point:   %d, %d\n", end.x, end.y);
        */

        if (start.x == end.x) { // same row
            for (int i = min(start.y, end.y), max_index = max(start.y, end.y); 
                    i <= max_index; 
                    i++) {
                map[i][start.x] += 1;
            }
        } else if (start.y == end.y) { // same col
            for (int i = min(start.x, end.x), max_index = max(start.x, end.x); 
                    i <= max_index; 
                    i++) {
                map[start.y][i] += 1;
            }
        } else { // diagonal
            // using linear function properties to discover line function
            int y_change = end.y - start.y;
            int x_change = end.x - start.x;
            //printf("diagonal. slope: %d/%d\n", y_change, x_change);


            for (int x = min(start.x, end.x), max_x = max(start.x, end.x);
                    x <= max_x;
                    x++) {
                int y = (y_change * (x - start.x))/x_change + start.y;
                //printf("for x %d the y is:\n", x);
                //printf("(%d * (%d - %d))/%d + %d = %d\n", y_change, x, start.x, x_change, end.y, y);
                map[y][x] += 1;
            }
        }

    }

    int overlapped_points = 0;
    for (int i = 0; i < DIAGRAM_SIDE; i++) {
        for (int j = 0; j < DIAGRAM_SIDE; j++) {
            if (map[i][j] > 1) {
                overlapped_points++;
            }
        }
    }

    printf("Overlapped points: %d\n", overlapped_points);
    //print_map(map);

    return 0;
}
