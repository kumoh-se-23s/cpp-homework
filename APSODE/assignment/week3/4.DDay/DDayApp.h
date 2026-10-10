//
// Created by leegu on 26. 10. 8..
//

#ifndef CPP_HOMEWORK_DDAYAPP_H
#define CPP_HOMEWORK_DDAYAPP_H

#include "DDay.h"
#include "Day.h"

struct Command {
    constexpr static int MAX_INPUT_LENGTH = 9;
    char input[MAX_INPUT_LENGTH];
    int length = 0;
    char resolved = '?';
};

class DDayApp {
    public:
        DDayApp();

        void run();

    private:
        constexpr static char PROMPT[] = ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";

        bool is_running;

        DDay dday;

        static Command resolve_input_command(Command command_struct);

        static Command input_command();

        void print_result() const;

        void print_error() const;

        bool stop();

        bool tomorrow();

        bool yesterday();

        bool setting_dday(const Command &command_struct);

        bool setting_new_day(const Command &command_struct);

};
#endif //CPP_HOMEWORK_DDAYAPP_H
