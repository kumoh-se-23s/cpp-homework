#include <iostream>

const int DEFAULT_RESULT_ARRAY_SIZE = 32;


int convert_dec_to_bin(int dec_num, bool result_array[]) {
    int index = 0;
    for (
        int number = dec_num; 
        number > 0; 
        number /= 2
    ) {
        result_array[index++] = number % 2;
    }

    return index;
}

void print_bin_array(bool target_array[], int last_index = -1) {
    for (
        int index = 0; 
        index < DEFAULT_RESULT_ARRAY_SIZE && last_index != -1 && index < last_index; 
        ++index
    ) {
        if (index != 0 && index % 4 == 0) {
            std::cout << " ";    
        }
                
        std::cout << target_array[index];
    }    
}


int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for (int loop = 0; loop < loop_amount; loop++) {
        int input_dec_num;
        std::cin >> input_dec_num;

        bool result_array[DEFAULT_RESULT_ARRAY_SIZE] = {};

        int last_index = convert_dec_to_bin(input_dec_num, result_array);
        print_bin_array(result_array, last_index);
    }

    return 0;
}