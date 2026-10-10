//
// Created by leegu on 26. 10. 9..
//

#ifndef CPP_HOMEWORK_STRINGAPP_H
#define CPP_HOMEWORK_STRINGAPP_H
#include "TextFormatter.h"

class StringApp {
    public:
        StringApp();

        void run();

    private:
        char input_container[TextFormatter::MAX_INPUT_LENGTH];

        int gap;

        void input();

        void print_alphabet_count() const;

        void print_capitalized_text() const;

        void print_encrypted_text() const;

};

#endif //CPP_HOMEWORK_STRINGAPP_H
