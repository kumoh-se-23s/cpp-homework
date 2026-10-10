//
// Created by leegu on 26. 10. 8..
//

#include "CharacterUtils.h"


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

    if (!is_numeric_character(maybe_signed_numeric[0]) && maybe_signed_numeric[0] != '-' && maybe_signed_numeric[0] !=
        '+') {
        return false;
    }

    for (int char_index = 1; maybe_signed_numeric[char_index] != '\0'; ++char_index) {
        if (!is_numeric_character(maybe_signed_numeric[char_index])) {
            return false;
        }
    }

    return true;
}


bool char_utils::is_correct_range(const int start, const int end, const int size) {
    const bool start_range_check = 0 <= start && start < size && start <= end;
    const bool end_range_check = 0 <= end && end < size && end >= start;

    return start_range_check && end_range_check;
}

bool char_utils::is_lower_alphabet(const char maybe_lower_alphabet) {
    return 'a' <= maybe_lower_alphabet && maybe_lower_alphabet <= 'z';
}

bool char_utils::is_upper_alphabet(const char maybe_upper_alphabet) {
    return 'A' <= maybe_upper_alphabet && maybe_upper_alphabet <= 'Z';
}

bool char_utils::is_alphabet_character(const char maybe_alphabet) {
    return is_lower_alphabet(maybe_alphabet) || is_upper_alphabet(maybe_alphabet);
}

bool char_utils::is_alphabet_only(const char maybe_alphabet_only[]) {
    for (int char_index = 0; maybe_alphabet_only[char_index] != '\0'; ++char_index) {
        if (!is_alphabet_character(maybe_alphabet_only[char_index])) {
            return false;
        }
    }

    return true;
}

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

int char_utils::get_char_array_length(const char char_array[]) {
    int length = 0;

    while (char_array[length] != '\0') {
        length++;
    }

    return length;
}

int char_utils::count_alphabet(const char text[]) {
    int count = 0;

    for (int char_index = 0; text[char_index] != '\0'; ++char_index) {
        if (is_alphabet_character(text[char_index])) {
            ++count;
        }
    }

    return count;
}

void char_utils::count_alphabet_by_array(const char text[], int count_array[26]) {
    for (int index = 0; index < 26; ++index) {
        count_array[index] = 0;
    }
    for (int char_index = 0; text[char_index] != '\0'; ++char_index) {
        if (is_alphabet_character(text[char_index])) {
            const char base_character = is_lower_alphabet(text[char_index]) ? 'a' : 'A';
            count_array[text[char_index] - base_character] += 1;
        }
    }
}

char char_utils::to_lower(const char maybe_alphabet) {
    if (!is_alphabet_character(maybe_alphabet) || is_lower_alphabet(maybe_alphabet)) {
        return maybe_alphabet;
    }

    return static_cast<char>(maybe_alphabet + ('a' - 'A'));
}

bool char_utils::to_lower(char text[]) {
    if (text[0] == '\0') {
        return true;
    }

    for (int char_index = 0; text[char_index] != '\0'; ++char_index) {
        text[char_index] = to_lower(text[char_index]);
    }

    return true;
}

char char_utils::to_upper(const char alphabet_char) {
    if (!is_alphabet_character(alphabet_char) || is_upper_alphabet(alphabet_char)) {
        return alphabet_char;
    }

    return static_cast<char>(alphabet_char - ('a' - 'A'));
}

bool char_utils::to_upper(char text[]) {
    if (text[0] == '\0') {
        return true;
    }

    for (int char_index = 0; text[char_index] != '\0'; ++char_index) {
        text[char_index] = to_upper(text[char_index]);
    }

    return true;
}

char char_utils::offset(const char processable_character, const int gap) {
    if (is_alphabet_character(processable_character)) {
        return offset_alphabet(processable_character, gap);
    }

    if (is_numeric_character(processable_character)) {
        return offset_numeric(processable_character, gap);
    }

    return processable_character;
}

char char_utils::offset_numeric(const char numeric_character, const int gap) {
    const int after_offset = ((numeric_character - '0' + gap) % 10 + 10) % 10;
    return static_cast<char>('0' + after_offset);
}

char char_utils::offset_alphabet(const char alphabet_character, const int gap) {
    const char base_character = is_lower_alphabet(alphabet_character) ? 'a' : 'A';
    const int after_offset = ((alphabet_character - base_character + gap) % 26 + 26) % 26;
    return static_cast<char>(base_character + after_offset);
}
