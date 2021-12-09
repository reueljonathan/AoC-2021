#include <stdio.h>
#include <stdlib.h>

int main() {

    int zero_quantity[12];
    char binary_number[13];
    char gamma_rate[13];
    char epsilon_rate[13];

    gamma_rate[12] = epsilon_rate[12] = '\0';

    for (int i = 0; i < 12; i++) {
        zero_quantity[i] = 0;
    }

    while(scanf("%s", binary_number) != EOF) {
        for (int i = 0; i < 12; i++) {
            if (binary_number[i] == '0') {
                zero_quantity[i]++;
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        if (zero_quantity[i] > 500) {
            gamma_rate[i] = '0';
            epsilon_rate[i] = '1';
        } else {
            gamma_rate[i] = '1';
            epsilon_rate[i] = '0';
        }
    }

    printf("gamma: %s (%ld)\n", gamma_rate, strtol(gamma_rate, NULL, 2));
    printf("epsilon: %s (%ld)\n", epsilon_rate, strtol(epsilon_rate, NULL, 2));


    return 0;
}
