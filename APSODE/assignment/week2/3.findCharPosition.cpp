//
// Created by apsode on 26. 9. 16..
//

#include <iostream>

const int MAX_INPUT_STRING_LENGTH = 101;
const int ALPHABET_START_POS_CONTAINER_SIZE = 26;

void printArray(int integer_array[], int max_length = 0) {
    for (int index = 0; index < max_length; ++index) {
        std::cout << integer_array[index] << " ";
    }
    std::cout << std::endl;
}

int main() {
    char input_string_container[MAX_INPUT_STRING_LENGTH] = {'\0'};
    int alphabet_start_pos_container[ALPHABET_START_POS_CONTAINER_SIZE] = {0};
    std::cin >> input_string_container;

    for (
        int char_index = 0;
        input_string_container[char_index] != '\0';
        ++char_index
    ) {
         if (alphabet_start_pos_container[input_string_container[char_index] - 'a'] == 0) {
             alphabet_start_pos_container[input_string_container[char_index] - 'a'] = char_index + 1;
         }
    }

    printArray(alphabet_start_pos_container, ALPHABET_START_POS_CONTAINER_SIZE);
    return 0;
}