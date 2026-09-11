#include <iostream>

void print_chars(int amount, char print_char) {
    for (int loop = 0; loop < amount; ++loop) {
        printf("%c", print_char);
    }
}


int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        // 값입력
        int side_length;
        std::cin >> side_length;

        // 상단
        for (int top_layer_lev = 1; top_layer_lev < side_length; ++top_layer_lev) {
            int blank_amount = side_length - top_layer_lev;
            int star_amount = side_length + 2*(top_layer_lev - 1);

            print_chars(blank_amount, ' ');
            print_chars(star_amount, '*');

            printf("\n");
        }
        // 중단
        for (int mid_layer_lev = 0; mid_layer_lev < side_length; ++mid_layer_lev) {
            int star_amount = side_length + 2*(side_length - 1);
            print_chars(star_amount, '*');
            printf("\n");
        }

        // 하단
        for (int bottom_layer_lev = side_length - 1; bottom_layer_lev > 0; --bottom_layer_lev) {
            int star_amount = side_length + 2*(bottom_layer_lev - 1);
            int blank_amount = side_length - bottom_layer_lev;
            print_chars(blank_amount, ' ');
            print_chars(star_amount, '*');
            printf("\n");
        }
    }

    return 0;
}
