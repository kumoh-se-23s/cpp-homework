//
// Created by leegu on 26. 10. 10..
//

#include "TextFormatter.h"

#include "CharacterUtils.h"

TextFormatter::TextFormatter()
    : container{} {
}

TextFormatter::TextFormatter(const char target_text[])
    : container{} {
    this->fill_container(target_text);
}

bool TextFormatter::is_valid_gap_range(const int gap) {
    return MIN_GAP <= gap && gap <= MAX_GAP;
}

void TextFormatter::fill_container(const char target_char_arr[]) {
    for (int char_index = 0; target_char_arr[char_index] != '\0' && char_index < MAX_INPUT_LENGTH; ++char_index) {
        this->container[char_index] = target_char_arr[char_index];
    }
}

int TextFormatter::get_first_alphabet_index() const {
    for (int char_index = 0; this->container[char_index] != '\0'; ++char_index) {
        if (char_utils::is_alphabet_character(this->container[char_index])) {
            return char_index;
        }
    }

    return -1;
}

TextFormatter& TextFormatter::capitalize_first() {
    const int first_alphabet_index = this->get_first_alphabet_index();
    char_utils::to_lower(this->container);

    this->container[first_alphabet_index] = char_utils::to_upper(this->container[first_alphabet_index]);

    return *this;
}

TextFormatter& TextFormatter::encrypt(const int gap) {
    if (!is_valid_gap_range(gap)) {
        return *this;
    }

    for (int char_index = 0; this->container[char_index] != '\0'; ++char_index) {
        this->container[char_index] = char_utils::offset(this->container[char_index], gap);
    }


    return *this;
}

void TextFormatter::build(char text[]) const {
    int index = 0;
    for (; this->container[index] != '\0' && index < MAX_INPUT_LENGTH; ++index) {
        text[index] = this->container[index];
    }
    text[index] = '\0';
}
