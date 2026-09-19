#include <iostream>



void print_operand_array(const int integer_array[], const int max_length) {
    for (int index = max_length - 1; index >= 0; --index) {
        std::cout << integer_array[index];
    }
    std::cout << std::endl;
}

void save_dec_num_in_array(const int dec_num, int array[]) {
    for (
        int number = dec_num, index = 0; 
        number > 0; 
        number /= 10, ++index
    ) {
        array[index] = number % 10;        
    }
}

int get_dec_num_length(int dec_num) {
    int length = 1;
    for (; dec_num / 10 != 0; dec_num /= 10) ++length;

    return length;
}

int main() {
    static constexpr int DEFAULT_OPERAND_MAX_LENGTH = 3;

    int right_operand[DEFAULT_OPERAND_MAX_LENGTH] = {};

    int left_operand;
    int raw_right_operand;
    
    std::cin >> left_operand;
    std::cin >> raw_right_operand;

    save_dec_num_in_array(raw_right_operand, right_operand);

    for (int index = 0; index < DEFAULT_OPERAND_MAX_LENGTH; ++index) {
        int calc_temp = left_operand * right_operand[index];
        int calc_temp_length = get_dec_num_length(calc_temp);
        int calc_temp_container[calc_temp_length];
        save_dec_num_in_array(calc_temp, calc_temp_container);

        print_operand_array(calc_temp_container, calc_temp_length);
    }

    int result = left_operand * raw_right_operand;
    int result_length = get_dec_num_length(result);
    int result_container[result_length];
    save_dec_num_in_array(result, result_container);

    print_operand_array(result_container, result_length);

    return 0;
}