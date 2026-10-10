//
// Created by leegu on 26. 10. 10..
//

#ifndef CPP_HOMEWORK_TEXTENCRYPTER_H
#define CPP_HOMEWORK_TEXTENCRYPTER_H

class TextFormatter {
    public:
        constexpr static int MAX_INPUT_LENGTH = 128;
        constexpr static int MIN_GAP = -25;
        constexpr static int MAX_GAP = 25;

        TextFormatter();

        TextFormatter(const char target_text[]);


        TextFormatter& capitalize_first();

        TextFormatter& encrypt(int gap);

        char* build();

    private:
        char container[MAX_INPUT_LENGTH];

        static bool is_valid_gap_range(int gap);

        void fill_container(const char target_char_arr[]);

        int get_first_alphabet_index() const;
};

#endif //CPP_HOMEWORK_TEXTENCRYPTER_H

