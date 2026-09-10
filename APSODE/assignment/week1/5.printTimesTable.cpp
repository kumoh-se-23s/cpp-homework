#include <iostream>

bool is_complete_number(int target) {
    int divisor = 0;
    for (int div_num = 2; div_num <= target; ++div_num) {
        if (target % div_num == 0) {
            divisor += div_num;
        }
    }

    return divisor == target;
}

bool is_prime_number(int target) {
    int divisor_count = 0;
    bool divisor_is_one = false;
    bool divisor_is_self = false;

    for ()
}


int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        int input_number;
        scanf_s("%d", &input_number);

        int temp = 0;
        for(int num = 2; num <= input_number; ++num) {
            if (input_number % num == 0) {
                temp += num;
            }
        }


    }

    return 0;
}
