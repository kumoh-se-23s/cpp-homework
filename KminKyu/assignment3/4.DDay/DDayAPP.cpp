#include<iostream>
#include "DDayAPP.h"

using namespace std;


void DDayAPP::run() {
    cout << dday;
    char answer[MAX_ANSWER_LENGTH];
    do {
        printMenu();
        cin.getline(answer,'\n');
        cin.clear();
        cin.ignore('\n');
        menu(answer);
    } while (toUpper(answer[0]) != 'Q');

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

void DDayAPP::menu(char command[]) const {
    for (int index = 0; index < MAX_ANSWER_LENGTH; ++index) {
        command[index] = toUpper(command[index]);
    }
    if (command[0] == 'Y') {
        this->dday.setYesterDay();

    } else if (command[0] == 'T') {
        this->dday.setTomarrow();

    } else if (command[0] == '+') {
        this->dday.setDDay(changeCharToInt(command, 1));

    } else if ('0' <= command[0] && command[0] <= '9') {
        if (this->dday.setStartDay(changeCharToInt(command, 0))) {
            cout << "*** ERROR\n";
        }

    } else if (command[0] != 'Q') {
        cout << "*** ERROR\n";
    }

}

int DDayAPP::changeCharToInt(const char character[], int startIndex) {
    int result = 0;
    for (int now = startIndex; character[now] != '\n'; ++now, result *= 10) {
        if ('0' <= character[now] && character[now] <= '9') {
            result = character[now] - '0';
        } else {
            return -1;
        }
    }
    return result;

}
