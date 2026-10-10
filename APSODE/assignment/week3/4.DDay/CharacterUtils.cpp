//
// Created by leegu on 26. 10. 8..
//

#include "CharacterUtils.h"


int char_utils::parse_integer(const char integer_text[], const int start, const int end) {
    int text_index = start;
    bool is_negative = false;

    if (integer_text[text_index] == '+' || integer_text[text_index] == '-') {
        is_negative = (integer_text[text_index] == '-');
        ++text_index;
    }

    int result = 0;

    for (; text_index <= end; ++text_index) {
        result = result * 10 + (integer_text[text_index] - '0');
    }

    return is_negative ? -result : result;
}

int char_utils::to_integer(const char integer_text[], const int start, const int end, const int size) {
    if (!is_correct_range(start, end, size)) {
        return -1;
    }

    return parse_integer(integer_text, start, end);
}

int char_utils::to_integer(const char integer_text[]) {
    int last_index = -1;

    while (integer_text[last_index + 1] != '\0') {
        ++last_index;
    }

    return parse_integer(integer_text, 0, last_index);
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

bool char_utils::is_numeric_only(const char maybe_numeric_only[]) {
    if (maybe_numeric_only[0] == '\0') {
        return false;
    }

    for (int char_index = 0; maybe_numeric_only[char_index] != '\0'; ++char_index) {
        if (!is_numeric_character(maybe_numeric_only[char_index])) {
            return false;
        }
    }

    return true;
}

bool char_utils::is_signed_numeric(const char maybe_signed_numeric[]) {
    if (maybe_signed_numeric[0] == '\0') {
        return false;
    }

    if (!is_numeric_character(maybe_signed_numeric[0]) && maybe_signed_numeric[0] != '-' && maybe_signed_numeric[0] != '+') {
        return false;
    }

    for (int char_index = 1; maybe_signed_numeric[char_index] != '\0'; ++char_index) {
        if (!is_numeric_character(maybe_signed_numeric[char_index])) {
            return false;
        }
    }

    return true;
}

