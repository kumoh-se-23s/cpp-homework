//
// Created by leegu on 26. 10. 8..
//

#ifndef CPP_HOMEWORK_CHARACTERUTILS_H
#define CPP_HOMEWORK_CHARACTERUTILS_H

namespace char_utils {
    int to_positive_integer(const char integer_text[], int size);
    int to_positive_integer(const char integer_text[], int start, int end, int size);
    int get_char_array_length(const char char_array[]);
    bool is_correct_range(int start, int end, int size);
}

#endif //CPP_HOMEWORK_CHARACTERUTILS_H

