#include "DDay.h"

#include <iostream>
#include <ostream>

using namespace std;

//default : 2026/10/01
DDay::DDay() {
    currentDate = Day();
}



void DDay::run() {
    //<< 2026/10/01 [D-day:+0] 2026/10/01 현재 상태 출력
    printCurrent();

    const int STR_MAX_LEN = 8;
    char input[STR_MAX_LEN + 1];

    //동작 파트
    bool isRun = true;

    while (isRun) {
        //>> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) :  메뉴 출력
        printMenu();

        cin >> input;
        for (int i = 0; i < STR_MAX_LEN; i++) {
            if (input[i] == '\0') {
                break;
            }
        }

        //입력 받은 문자열 길이 구하기
        //1글자 << q t y(일반적인 상황에서)
        //8글자 << + - yyyymmdd
        //q1234567, +123q456, ++123456, 12q34567 같은 예외 고려하기
        int currentLength = 0;
        for (;input[currentLength] != '\0'  && currentLength < STR_MAX_LEN + 1; currentLength++) {}

        //q, t, y 등은 1글자만 판별해도됨
        if (currentLength == 1) {
            switch (input[0]) {
                case 'Q': case 'q':
                    cout << "=== END ===";
                    isRun = false;
                    break;

                case 'T': case 't':
                    ++currentDate;
                    printCurrent();
                    break;

                case 'Y': case 'y':
                    --currentDate;
                    printCurrent();
                    break;

                default:
                    printError();
                    break;
            }
        }
        else {
            switch (input[0]) {
                case '+': case '-': {
                    int offset = 0;
                    int sign = (input[0] == '+') ? 1 : -1;
                    bool isNumber = true;

                    //배열 index 1부터 숫자 검사
                    for (int i = 1; i < currentLength; i++) {
                        if ('0' <= input[i] && input[i] <= '9') {
                            offset = offset * 10 + (input[i] - '0');
                        }
                        //+123q456같은 문자면 else -> isValid = false;
                        else {
                            isNumber = false;
                            break;
                        }
                    }
                    if (isNumber) {
                        dDayValue = offset * sign;
                        printCurrent();
                    }else {
                        printError();
                    }
                    break;
                }
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9': {
                    bool isAllNum = true;
                    for (int i = 0; i < currentLength; i++) {
                        if (input[i] < '0' || input[i] > '9') {
                            isAllNum = false;
                            break;
                        }
                    }
                    if (isAllNum) {
                        int year = (input[0] - '0') * 1000 + (input[1] - '0') * 100 + (input[2] - '0') * 10 + (input[3] - '0');
                        int month = (input[4] - '0') * 10 + (input[5] - '0');
                        int day = (input[6] - '0') * 10 + (input[7] - '0');

                        if (calc.isValidDate(year, month, day)) {
                            currentDate = Day(year, month, day);
                            printCurrent();
                        }
                        else {
                            printError();
                        }
                    }
                    else {
                        printError();
                    }
                    break;
                }
                default: {
                    printError();
                    break;
                }
            }
        }
    }
}

void DDay::printCurrent() {
    Day targetDate = currentDate + dDayValue;

    cout << "<< " << currentDate << " [D-day:";
    if (dDayValue >= 0 ) {
        cout << "+";
    }
    cout << dDayValue << "] " << targetDate << endl;
}

void DDay::printMenu() {
    cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";
}

void DDay::printError() {
    cout << "*** ERROR" << endl;
    printCurrent();
}