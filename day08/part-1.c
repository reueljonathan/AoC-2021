#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {

    char ** digits = (char **) malloc(sizeof(char *) * 10);
    char ** displayed_digits = (char **) malloc(sizeof(char *) * 4);
    unsigned int num_appearances = 0, str_len;
    int scanf_result;

    for (int i = 0; i < 10; i++) {
        digits[i] = (char *) malloc(sizeof(char) * 8); // 7 digits + '0'
    }

    for (int i = 0; i < 4; i++) {
        displayed_digits[i] = (char *) malloc(sizeof(char) * 8); // 7 digits + '0'
    }

    bool has_input = true;
    while (has_input) {
        for (int i = 0; i < 10; i++) {
            scanf_result = scanf("%s", digits[i]);    


            if (scanf_result == EOF) {
                has_input = false;
                break;
            } else {
 //               printf("d: %s ", digits[i]);
            }
        }
  //      puts("");

        if (!has_input) break;

        scanf(" | "); // discard separator

        for (int i = 0; i < 4; i++) { // part 1
            scanf("%s", displayed_digits[i]);    
            str_len = strlen(displayed_digits[i]);

//            printf("dd: %s (%u) ", displayed_digits[i], str_len);

            switch (str_len) {
                case 2: // digit 1
                case 3: // digit 7
                case 4: // digit 4
                case 7: // digit 8
                {
                    num_appearances++;
                } break;
            }
        }
    }

    printf("num appearances: %u\n", num_appearances);

    return 0;
}
