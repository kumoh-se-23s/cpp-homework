//
// Created by leegu on 26. 10. 8..
//

#include "CharacterUtils.h"

int char_utils::to_positive_integer(const char integer_text[], const int size) {
    return to_positive_integer(integer_text, 0, size - 1, size);
}

int char_utils::to_positive_integer(const char integer_text[], const int start, const int end, const int size) {
    if (!is_correct_range(start, end, size)) {
        return -1;
    }

    int result = 0;
    int base = 1;
    for (int char_index = end; char_index >= start; --char_index) {
        result += (integer_text[char_index] - '0') * base;
        base *= 10;
    }

    return result;
}

bool char_utils::is_correct_range(const int start, const int end, const int size) {
    const bool start_range_check = 0 <= start && start < size && start <= end;
    const bool end_range_check = 0 <= end && end < size && end >= start;

    return start_range_check && end_range_check;
}

int char_utils::get_char_array_length(const char char_array[]) {
    int length = 0;

    while (char_array[length] != '\0') {
        length++;
    }

    return length;
}
