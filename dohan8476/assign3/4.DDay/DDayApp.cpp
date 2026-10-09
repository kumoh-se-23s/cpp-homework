#include "DDayApp.h"

#include <iostream>
#include <ostream>

using namespace std;

//default : 2026/10/01
DDayApp::DDayApp() {
    currentDate = Day();
}

void DDayApp::run() {
    printCurrent();

    const int STR_MAX_LEN = 8;
    char input[STR_MAX_LEN + 1];

    bool isRun = true;

    while (isRun) {
        printMenu();
        cin.getline(input, STR_MAX_LEN + 1);
        //getline에서 범위를 초과하면 fail
        if (cin.fail()) {
            printError();
            cin.clear();
            //버퍼에 남은것들 비워주기
            cin.ignore(1000, '\n');
            continue;
        }
        //현재 입력 받은거 길이 구하기
        int currentLength = 0;
        for (; currentLength < STR_MAX_LEN + 1 && input[currentLength] != '\0'; currentLength++) {}

        if ((input[0] == 'Q' || input[0] == 'q') && currentLength == 1) {
            cout << "=== END ===";
            isRun = false;
        }
        else {
            processCommand(input, currentLength);
        }
    }
}
//메뉴 목록 처리
void DDayApp::processCommand(const char input[], const int length) {
    if (isDigit(input[0])) {
        handleMoveDate(input, length);
        return;
    }

    if (length == 1) {
        switch (input[0]) {
            case 'T': case 't':
                ++currentDate;
                printCurrent();
                break;

            case 'Y': case 'y':
            {
                Day temp = currentDate;
                --temp;
                isValidNewDate(temp, dDayValue);
                break;
            }
            default: {
                printError();
                break;
            }
        }
    } else {
        switch (input[0]) {
            case '+': case '-': {
                handleSetDDay(input, length);
                break;
            }
            default: {
                printError();
                break;
            }
        }
    }
}
// +, -로 DDay 세팅
void DDayApp::handleSetDDay(const char input[], const int length) {
    int offset = 0;
    int sign = (input[0] == '+') ? 1 : -1;

    //배열 index 1부터 숫자 검사
    for (int i = 1; i < length; i++) {
        if (!isDigit(input[i])) {
            printError();
            return;
        }
        offset = offset * 10 + (input[i] - '0');
    }

    isValidNewDate(currentDate, offset * sign);
}
//yyyymmdd로 세팅
void DDayApp::handleMoveDate(const char input[], const int length) {

    for (int i = 0; i < length; i++) {
        if (!isDigit(input[i])) {
            printError();
            return;
        }
    }
    int year = parseInt(input, 0, 4);
    int month = parseInt(input, 4, 2);
    int day = parseInt(input, 6, 2);

    Day tempDate(year, month, day);
    isValidNewDate(tempDate, dDayValue);
}


void DDayApp::printCurrent() {
    Day targetDate = currentDate + dDayValue;

    cout << "<< " << currentDate << " [D-day:";
    if (dDayValue >= 0) {
        cout << "+";
    }
    cout << dDayValue << "] " << targetDate << endl;
}

void DDayApp::printMenu() {
    cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";
}

void DDayApp::printError() {
    cout << "*** ERROR" << endl;
    printCurrent();
}

bool DDayApp::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool DDayApp::isValidNewDate(Day newDay, const int newDDayValue) {
    if (!calc.isValidDate(newDay.getYear(), newDay.getMonth(), newDay.getDay())) {
        printError();
        return false;
    }

    Day target = newDay + newDDayValue;
    if (!calc.isValidDate(target.getYear(), target.getMonth(), target.getDay())) {
        cout << "Cannot subtract days: Date must be after 1896/01/01" << endl;
        printCurrent();
        return false;
    }

    currentDate = newDay;
    dDayValue = newDDayValue;
    printCurrent();
    return true;
}

int DDayApp::parseInt(const char input[], const int start, const int length) {
    int result = 0;
    for (int i = start; i < start + length; i++) {
        result = result * 10 + (input[i] - '0');
    }
    return result;
}
