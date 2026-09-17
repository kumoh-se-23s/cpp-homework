#include <iostream>

int get_dec_num_length(int dec_num) {
    int length = 1;
    for (; dec_num / 10 != 0; dec_num /= 10) ++length;

    return length;
}

void print_integer_array(int integer_array[], int max_length = 0) {
    for (int index = max_length - 1; index >= 0; --index) {
        std::cout << integer_array[index];
    }
    std::cout << std::endl;
}

void print_char_array(char char_array[]) {
    for (int index = 0; char_array[index] != '\0'; ++index) {
        std::cout << char_array[index];
    }
    std::cout << std::endl;
}

int get_digit_length(int target) {
    int count = target < 0 ? 1 : 0;
    for (; target != 0; target /= 10) ++count;
    return count;
}

int main() {
    std::cout << get_digit_length(2);


    return 0;
}