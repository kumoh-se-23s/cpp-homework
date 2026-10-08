//
// Created by leegu on 26. 10. 8..
//

#include "CharacterUtils.h"


int char_utils::to_ranged_integer(const char integer_text[], const int start, const int end, const int size) {
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

int char_utils::to_integer(const char integer_text[]) {
    int text_index = 0;
    bool is_negative = false;

    if (integer_text[text_index] == '+' || integer_text[text_index] == '-') {
        is_negative = (integer_text[text_index] == '-');
        ++text_index;
    }

    int result = 0;

    for (; integer_text[text_index] != '\0'; ++text_index) {
        result = result * 10 + (integer_text[text_index] - '0');
    }

    return is_negative ? -result : result;
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

bool char_utils::is_numeric_character(const char maybe_numeric) {
    return '0' <= maybe_numeric && maybe_numeric <= '9';
}

bool char_utils::is_numeric_only(char maybe_numeric_only[]) {
    if (maybe_numeric_only[0] == '\0') {
        return false;
    }

    // if (!is_numeric_character(maybe_numeric_only[0]) && maybe_numeric_only[0] != '-' && maybe_numeric_only[0] != '+') {
    //     return false;
    // }

    for (int char_index = 0; maybe_numeric_only[char_index] != '\0'; ++char_index) {
        if (!is_numeric_character(maybe_numeric_only[char_index])) {
            return false;
        }
    }

    return true;
}

