#include <iostream>

enum NumType {
    DEFAULT,
    PRIME,
    COMPLETE
};

int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for (int loop = 0; loop < loop_amount; ++loop) {
        int input_number;
        std::cin >> input_number;
        NumType before_num_type = DEFAULT;
        NumType current_num_type = DEFAULT;

        for (int current_number = 1; current_number <= input_number; ++current_number) {
            int total_divisor = 1;
            for (int divisor = 2; divisor < current_number; ++divisor) {
                if (current_number % divisor == 0) {
                    total_divisor += divisor;
                }
            }

            if (current_number >= 10 && total_divisor == 1) {
                current_num_type = PRIME;
                if (before_num_type != current_num_type) printf(before_num_type == DEFAULT ? "[P]" : "\n[P]");

                printf(" %d", current_number);
            } else if (current_number > 1 && total_divisor == current_number) {
                current_num_type = COMPLETE;
                if (before_num_type != current_num_type) printf(before_num_type == DEFAULT ? "[C] %d = 1" : "\n[C] %d = 1", current_number);

                for (int divisor = 2; divisor < current_number; ++divisor) {
                    if (current_number % divisor == 0) printf(" + %d", divisor);
                }
            }

            before_num_type = current_num_type;
        }
        std::cout << std::endl;
    }

    return 0;
}
