#include "DDayApp.h"

#include <iostream>
#include <ostream>

using namespace std;

//default : 2026/10/01
DDayApp::DDayApp() {
    currentDate = Day();
}

void DDayApp::run() {
    //<< 2026/10/01 [D-day:+0] 2026/10/01 현재 상태 출력
    printCurrent();

    const int STR_MAX_LEN = 8;
    char input[STR_MAX_LEN + 1];

    //동작 파트
    bool isRun = true;

    while (isRun) {
        //>> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) :  메뉴 출력
        printMenu();

        cin.getline(input, STR_MAX_LEN + 1);

        //getline에서 범위를 초과하면 fail
        if (cin.fail()) {
            printError();
            //현재꺼 clear
            cin.clear();
            //버퍼에 남은것들 '\n' 만날 때 까지 비워주기
            cin.ignore(1000, '\n');
            continue;
        }

        int currentLength = 0;
        //길이 반환
        for (; currentLength < STR_MAX_LEN + 1 && input[currentLength] != '\0'; currentLength++) {}

        //1글자 << q t y(일반적인 상황에서)
        //8글자 << + - yyyymmdd
        //q1234567, +123q456, 12q34567 같은 예외 고려하기

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

                case 'Y': case 'y': {
                    Day temp = currentDate;
                    --temp;
                    Day target = temp + dDayValue;

                    //00010101 에서 y하면 오류,,,,,
                    if (calc.isValidDate(temp.getYear(), temp.getMonth(), temp.getDay()) &&
                        calc.isValidDate(target.getYear(), target.getMonth(), target.getDay())) {
                        currentDate = temp;
                        printCurrent();
                        } else {
                            printError();
                        }
                    break;
                }

                default:
                    printError();
                    break;
            }
        }
        else if (currentLength <= 8){
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
                        int tempDDayValue = offset * sign;
                        Day target = currentDate + tempDDayValue;

                        //00010101에서 -50 입력하면 뚫리는거 해결완료
                        if (calc.isValidDate(target.getYear(), target.getMonth(), target.getDay())) {
                            dDayValue = tempDDayValue;
                            printCurrent();
                        }
                        else {
                            printError();
                        }

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
                            //이미 dDayValue가 -50으로 세팅되어 있는 상태에서
                            //00010101로가면 생기는 버그 해결완료
                            Day tempDate(year, month, day);
                            Day target = tempDate + dDayValue;

                            if (calc.isValidDate(target.getYear(), target.getMonth(), target.getDay())) {
                                currentDate = tempDate;
                                printCurrent();
                            }
                            else {
                                printError();
                            }
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

void DDayApp::printCurrent() {
    Day targetDate = currentDate + dDayValue;

    cout << "<< " << currentDate << " [D-day:";
    if (dDayValue >= 0 ) {
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