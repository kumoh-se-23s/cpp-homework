#include <iostream>

int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        int input_number;
        int pos = 1;

        std::cin >> input_number;
        for (; input_number / 10 != 0; input_number /= 10) ++pos;
        printf("%d @ [%d]\n", input_number, pos);
    }

    return 0;
}
