/* reueljonathan 2021-12-09 00:45 AM
 *
 * Ok, so if you are trying to read this code, expect a lot of bad code written
 * here. I'm am tired right now and just wanted to solve this.
 *
 * I'll try to explain briefly the solution:
 *
 * Each digit of the seven-segment display has (wait for it) seven parts. Each
 * one of the seven segments can be either on or off, so they can be represented
 * by a bit.
 *
 * Ex:
 * digit in input: ce
 * active bits: c, e
 *
 * in binary representation:
 *
 * g f e d c b a
 * 0 0 1 0 1 0 0 
 *
 * so the bit mask of the digit ce is 0010100.
 *
 * by the size of digit string (2 chars), we know that is one of the specific cases,
 * as the digit 1 is the one that have 2 active bits.
 *
 * We repeat this process to discover the masks of all digits with unique number of
 * active bits:
 * - 1 (2 active bits)
 * - 4 (4 active bits)
 * - 7 (3 active bits)
 * - 8 (7 active bits)
 *
 * Now we start the deductions from the other digits...
 * 
 * - From the digits that have 5 active bits (2, 3, 5), only the 3 DOES HAVE both bits
 *   from the digit 1. So if (mask & mask_one) == mask_one, we found the 3!
 *
 * - From the digits that have 6 active bits (0, 6, 9), only the digit 6 DOES NOT have
 *   the bits from the mask of the digit 1. So if (mask & mask_one) != mask_one, we
 *   found the 6! oh yeah! o/
 *
 * I hope you got the idea. For the rest of the missing digits, jump directly to the
 * loop at line 193.
 *
 * This is a veeery good and challenging question.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum MASKS {
    A = 1 << 0,
    B = 1 << 1,
    C = 1 << 2,
    D = 1 << 3,
    E = 1 << 4,
    F = 1 << 5,
    G = 1 << 6,
};

unsigned int convert_char_to_mask(char c) {
    switch (c) {
        case 'a' :
            return A;
        case 'b' :
            return B;
        case 'c' :
            return C;
        case 'd' :
            return D;
        case 'e' :
            return E;
        case 'f' :
            return F;
        case 'g' :
            return G;
    }
}

unsigned int convert_digit_to_bit_mask(char * str_digit) {
    size_t len = strlen(str_digit);
    unsigned int result = 0;

    for (int i = 0; i < len; i++) {
        result |= convert_char_to_mask(str_digit[i]);
    }

    return result;
}

void print_binary(unsigned int n, unsigned int mask) {
    printf("%u ", n);

    printf("%c", mask & 0x40 ? '1' : '0');
    printf("%c", mask & 0x20 ? '1' : '0');
    printf("%c", mask & 0x10 ? '1' : '0');
    printf("%c", mask & 0x08 ? '1' : '0');
    printf("%c", mask & 0x04 ? '1' : '0');
    printf("%c", mask & 0x02 ? '1' : '0');
    printf("%c\n", mask & 0x01 ? '1' : '0'); 
}


int main() {
    char ** digits = (char **) malloc(sizeof(char *) * 10);
    char ** displayed_digits = (char **) malloc(sizeof(char *) * 4);
    unsigned int num_appearances = 0, str_len;
    long unsigned int sum_outputs = 0;
    int scanf_result;

    unsigned int
        mask_zero,
        mask_one,
        mask_two,
        mask_three,
        mask_four,
        mask_five,
        mask_six,
        mask_seven,
        mask_eight,
        mask_nine,
        mask;

    for (int i = 0; i < 10; i++) {
        digits[i] = (char *) malloc(sizeof(char) * 8); // 7 digits + '0'
    }

    for (int i = 0; i < 4; i++) {
        displayed_digits[i] = (char *) malloc(sizeof(char) * 8); // 7 digits + '0'
    }

    bool has_input = true;
    while (has_input) {
        // read digits representations
        for (int i = 0; i < 10; i++) {
            scanf_result = scanf("%s", digits[i]);    

            if (scanf_result == EOF) {
                has_input = false;
                break;
            }
        }

        if (!has_input) break;

        // guess with digit goes with which mask
        // first discover the masks of the numbers that have specific number of wirings
        

        for (int i = 0; i < 10; i++) {
            str_len = strlen(digits[i]);

            switch (str_len) {
                case 2:
                    mask_one = convert_digit_to_bit_mask(digits[i]);
                    break;
                case 3:
                    mask_seven = convert_digit_to_bit_mask(digits[i]);
                    break;
                case 4:
                    mask_four = convert_digit_to_bit_mask(digits[i]);
                    break;
                case 7:
                    mask_eight = convert_digit_to_bit_mask(digits[i]);
                    break;
            }
        }

        // now discover which of them are the mask of 3 and 6
        for (int i = 0; i < 10; i++) {
            str_len = strlen(digits[i]);
            mask = convert_digit_to_bit_mask(digits[i]);

            switch (str_len) {
                case 5: {
                    if ((mask & mask_one) == mask_one) {
                        mask_three = mask;
                    }

                    break;
                }
                case 6: {
                    if ((mask & mask_one) != mask_one) {
                        mask_six = mask;
                    }
                    break;
                }
            }
        }

        // now discover which of them are the mask of 0, 2, 5 and 9
        for (int i = 0; i < 10; i++) {
            str_len = strlen(digits[i]);
            mask = convert_digit_to_bit_mask(digits[i]);

            switch (str_len) {
                case 5: {
                    if (mask != mask_three) { // we already discovered 3 before
                        if ((mask & mask_six) == mask) {
                            mask_five = mask;
                        } else {
                            mask_two = mask;        
                        }
                    }

                    break;
                }
                case 6: {
                    if (mask != mask_six) {
                        if ((mask & mask_three) == mask_three) {
                            mask_nine = mask;
                        } else {
                            mask_zero = mask;
                        }
                    }
                }
            }
        }

        scanf(" | "); // discard separator

        /*
        printf("  gfedcba\n");
        print_binary(0, mask_zero);
        print_binary(1, mask_one)  ;
        print_binary(2, mask_two)  ;
        print_binary(3, mask_three);
        print_binary(4, mask_four) ;
        print_binary(5, mask_five) ;
        print_binary(6, mask_six)  ;
        print_binary(7, mask_seven);
        print_binary(8, mask_eight);
        print_binary(9, mask_nine) ;
        */

        unsigned int output = 0;

        for (int i = 0; i < 4; i++) { // part 1
            scanf("%s", displayed_digits[i]);    
            str_len = strlen(displayed_digits[i]);
            mask = convert_digit_to_bit_mask(displayed_digits[i]);

            /*

            if      (mask == mask_zero)  printf(" 0");
            else if (mask == mask_one)   printf(" 1");
            else if (mask == mask_two)   printf(" 2");
            else if (mask == mask_three) printf(" 3");
            else if (mask == mask_four)  printf(" 4");
            else if (mask == mask_five)  printf(" 5");
            else if (mask == mask_six)   printf(" 6");
            else if (mask == mask_seven) printf(" 7");
            else if (mask == mask_eight) printf(" 8");
            else if (mask == mask_nine)  printf(" 9");
            */

            if      (mask == mask_zero)  output = (output * 10) + 0;
            else if (mask == mask_one)   output = (output * 10) + 1;
            else if (mask == mask_two)   output = (output * 10) + 2;
            else if (mask == mask_three) output = (output * 10) + 3;
            else if (mask == mask_four)  output = (output * 10) + 4;
            else if (mask == mask_five)  output = (output * 10) + 5;
            else if (mask == mask_six)   output = (output * 10) + 6;
            else if (mask == mask_seven) output = (output * 10) + 7;
            else if (mask == mask_eight) output = (output * 10) + 8;
            else if (mask == mask_nine)  output = (output * 10) + 9;

        }
        /*
        puts("");
        */
        sum_outputs += output;
    }

    printf("sum outputs: %llu\n", sum_outputs);

    return 0;
}
