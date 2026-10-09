//
// Created by leegu on 26. 10. 8..
//

#ifndef CPP_HOMEWORK_CHARACTERUTILS_H
#define CPP_HOMEWORK_CHARACTERUTILS_H

namespace char_utils {
    bool is_numeric_character(char maybe_numeric);

    bool is_numeric_only(char maybe_numeric_only[]);

    bool is_signed_numeric(char maybe_signed_numeric[]);

    int to_ranged_integer(const char integer_text[], int start, int end, int size);

    int to_integer(const char integer_text[]);

    int get_char_array_length(const char char_array[]);

    bool is_correct_range(int start, int end, int size);
}

#endif //CPP_HOMEWORK_CHARACTERUTILS_H

