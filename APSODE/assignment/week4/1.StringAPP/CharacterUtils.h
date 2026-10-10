//
// Created by leegu on 26. 10. 8..
//

#ifndef CPP_HOMEWORK_CHARACTERUTILS_H
#define CPP_HOMEWORK_CHARACTERUTILS_H

namespace char_utils {
    bool is_numeric_character(char maybe_numeric);

    bool is_numeric_only(const char maybe_numeric_only[]);

    bool is_signed_numeric(const char maybe_signed_numeric[]);

    bool is_lower_alphabet(char maybe_lower_alphabet);

    bool is_upper_alphabet(char maybe_upper_alphabet);

    bool is_alphabet_character(char maybe_alphabet);

    bool is_alphabet_only(const char maybe_alphabet_only[]);

    bool is_correct_range(int start, int end, int size);

    int to_integer(const char integer_text[]);

    int to_integer(const char integer_text[], int start, int end, int size);

    int parse_integer(const char integer_text[], int start, int end);

    int get_char_array_length(const char char_array[]);

    int count_alphabet(const char text[]);

    void count_alphabet_by_array(const char text[], int count_array[]);

    char to_lower(char maybe_alphabet);

    bool to_lower(char text[]);

    char to_upper(char alphabet_char);

    bool to_upper(char text[]);

    char offset(char processable_character, int gap);

    char offset_numeric(char numeric_character, int gap);

    char offset_alphabet(char alphabet_character, int gap);
}

#endif //CPP_HOMEWORK_CHARACTERUTILS_H

