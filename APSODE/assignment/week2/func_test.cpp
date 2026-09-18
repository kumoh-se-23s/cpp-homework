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

int get_max(const int num1, const int num2) {
    return num1 >= num2 ? num1 : num2;
}

int get_min(const int num1, const int num2) {
    return num1 <= num2 ? num1 : num2;
}

int calc_gcd(const int num1, const int num2) {
    const int larger = get_max(num1, num2);
    const int smaller = get_min(num1, num2);

    if (smaller == 0) {
        return larger;
    }

    const int temp = larger % smaller;
    if (temp == 0) {
        return smaller;
    }

    return calc_gcd(smaller, temp);
}

int main() {
    std::cout << calc_gcd(1, 0);

    return 0;
}