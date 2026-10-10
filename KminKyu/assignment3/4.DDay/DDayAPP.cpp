#include<iostream>
#include "DDayAPP.h"

using namespace std;


void DDayAPP::run() {
    cout << dday;
    char answer[MAX_ANSWER_LENGTH] = {};

    do {
        printMenu();
        cin.getline(answer,MAX_ANSWER_LENGTH);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        menu(answer);
        if (toUpper(answer[0]) != 'Q' || answer[1] != '\0') {
            cout << dday;
        }


    } while (toUpper(answer[0]) != 'Q' || answer[1] != '\0');

    cout << "=== END ===";
}

void DDayAPP::printMenu() {
    cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";
}

char DDayAPP::toUpper(char alphabet) {
    if ('a' <= alphabet && alphabet <= 'z') {
        return (alphabet - 'a' + 'A');
    } else {
        return alphabet;
    }
}

void DDayAPP::menu(char command[]) {
    int result;
    for (int index = 0; index < MAX_ANSWER_LENGTH; ++index) {
        command[index] = toUpper(command[index]);
    }
    if (command[0] == 'Y' && command[1] == '\0') {
        this->dday.setYesterday();

    } else if (command[0] == 'T' && command[1] == '\0') {
        this->dday.setTomorrow();

    } else if (command[0] == '+' || command[0] == '-') {
        result = changeCharToInt(command, 1);

        if (result == -1) {
            cout << "*** ERROR\n";
            return;
        }

        if (command[0] == '-') {
            result = -result;
        }
        this->dday.setDDay(result);
    } else if ('0' <= command[0] && command[0] <= '9') {
        result = changeCharToInt(command, 0);
        if (result == -1 || !this->trySetStartDay(result)) {
            cout << "*** ERROR\n";
        }

    } else if (command[0] != 'Q' || command[1] != '\0') {
        cout << "*** ERROR\n";
    }
}

int DDayAPP::changeCharToInt(const char character[], int startIndex) {
    int result = 0;
    if (character[startIndex] == '\0') {
        return -1;
    }
    for (int now = startIndex; character[now] != '\0'; now++) {
        if ('0' <= character[now] && character[now] <= '9') {
            result *= 10;
            result += character[now] - '0';
        } else {
            return -1;
        }
    }
    return result;
}

bool DDayAPP::trySetStartDay(int yyyymmdd) {
    int year = yyyymmdd / 10000;
    yyyymmdd %= 10000;
    int month = yyyymmdd / 100;
    int day = yyyymmdd % 100;
    return this->dday.setStartDay(year, month, day);
}
