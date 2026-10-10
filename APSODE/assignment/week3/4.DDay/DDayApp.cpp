//
// Created by leegu on 26. 10. 8..
//

#include "DDayApp.h"
#include <iostream>

#include "CharacterUtils.h"

DDayApp::DDayApp() : is_running(true), dday(DDay()){
}

void DDayApp::run() {
    std::cout << "<< " << this->dday << std::endl;
    while (is_running) {
        const Command resolved_command = resolve_input_command(input_command());
        bool is_success = true;

        switch (resolved_command.resolved) {
            case '1' : // resolve(format:YYYYMMDD) => '1'
                is_success = this->setting_new_day(resolved_command);
                break;
            case '2' : // resolve('t' | 'T') => '2'
                is_success = this->tomorrow();
                break;
            case '3' : // resolve('y' | 'Y') => '3'
                is_success = this->yesterday();
                break;
            case '4' : // resolve('+' | '-') => '1'
                is_success = this->setting_dday(resolved_command);
                break;
            case '5' : // resolve('q' | 'Q') => '1'
                is_success = this->stop();
                break;

            default: // resolve fail => '?'
                is_success = false;
                break;
        }

        if (!is_running) {
            break;
        }

        if (is_success) {
            this->print_result();
        } else {
            this->print_error();
        }
    }

    std::cout << "=== END ===";
}

Command DDayApp::resolve_input_command(Command command_struct) {
    const char command_prefix = command_struct.input[0];

    if (char_utils::is_numeric_only(command_struct.input)) {
        command_struct.resolved = '1';
    } else if (command_prefix == 't' || command_prefix == 'T') {
        command_struct.resolved = '2';
    } else if (command_prefix == 'y' || command_prefix == 'Y') {
        command_struct.resolved = '3';
    } else if (command_prefix == '+' || command_prefix == '-') {
        command_struct.resolved = '4';
    } else if (command_prefix == 'q' || command_prefix == 'Q') {
        command_struct.resolved = '5';
    } else {
        command_struct.resolved = '?';
    }

    return command_struct;
}

Command DDayApp::input_command() {
    Command command{};
    std::cout << PROMPT;
    std::cin.getline(command.input, Command::MAX_INPUT_LENGTH);

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    return command;
}

void DDayApp::print_result() const {
    std::cout << "<< " << this->dday << std::endl;
}

void DDayApp::print_error() const {
    std::cout << "*** ERROR" << std::endl;
    this->print_result();
}

bool DDayApp::stop() {
    this->is_running = false;

    return true;
}

bool DDayApp::tomorrow() {
    this->dday.tomorrow();

    return true;
}

bool DDayApp::yesterday() {
    this->dday.yesterday();

    return true;
}

bool DDayApp::setting_dday(const Command &command_struct) {
    if (!char_utils::is_signed_numeric(command_struct.input)) {
        return false;
    }

    this->dday.set_dday(char_utils::to_integer(command_struct.input));

    return true;
}

bool DDayApp::setting_new_day(const Command &command_struct) {
    const Day maybe_valid_day = Day(command_struct.input);

    if (!Day::is_valid_day(maybe_valid_day)) {
        return false;
    }

    this->dday.set_new_day(maybe_valid_day);
    return true;
}