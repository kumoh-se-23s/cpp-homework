//
// Created by leegu on 26. 10. 10..
//
#include "CharSequence.h"

std::ostream& operator<<(std::ostream &output_stream, const CharSequence &char_sequence) {
    for (int index = 0; index < char_sequence.length(); ++index) {
        output_stream << char_sequence.at(index);
    }
    return output_stream;
}