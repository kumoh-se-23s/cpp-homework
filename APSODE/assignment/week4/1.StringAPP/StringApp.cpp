//
// Created by leegu on 26. 10. 9..
//

#include "StringApp.h"

#include <iostream>

#include "CharacterUtils.h"
#include "TextFormatter.h"
#include "4.DDay/CharacterUtils.h"


StringApp::StringApp() : gap(0) {
}

void StringApp::run() {
    this->input();

    this->print_alphabet_count();
    this->print_capitalized_text();
    this->print_encrypted_text();
}

void StringApp::input() {
    std::cin.getline(this->input_container, TextFormatter::MAX_INPUT_LENGTH);

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cin >> this->gap;
}

void StringApp::print_alphabet_count() const {
    int alphabet_count_array[26] = {};
    char_utils::count_alphabet_by_array(this->input_container, alphabet_count_array);

    for (int alphabet_index = 0; alphabet_index < 26; ++alphabet_index) {
        if (alphabet_count_array[alphabet_index] > 0) {
            std::cout << "[" << static_cast<char>('a' + alphabet_index) << ":" << alphabet_count_array[alphabet_index] << "] ";
        }
    }

    std::cout << std::endl;
}

void StringApp::print_capitalized_text() const {
    TextFormatter formatter = TextFormatter(this->input_container);
    std::cout << formatter.capitalize_first().build() << std::endl;
}

void StringApp::print_encrypted_text() const {
    TextFormatter formatter = TextFormatter(this->input_container);
    std::cout << formatter.encrypt(gap).build();
}
