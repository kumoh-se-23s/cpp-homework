#include <iostream>


int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        double double_number;
        
        std::cin >> double_number;

        int decimal_part = static_cast<int>(double_number);

        double real_part = double_number - decimal_part;

        std::cout << decimal_part << " + " << real_part << std::endl;
    }

    return 0;
}
